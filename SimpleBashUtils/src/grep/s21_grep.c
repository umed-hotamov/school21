#define _GNU_SOURCE 1

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPTS ":f:e:ivclnhso"

enum errors {
  ERR_MISSING_ARGUMENT = 1,
  ERR_UKNOWN_OPTION,
  ERR_GET_PATTERN,
  ERR_NO_FILE
};

struct grep_opts {
  bool pattern;
  bool ignore_case;
  bool invert_match;
  bool count;
  bool files_with_matches;
  bool line_number;
  bool print_filename;
  bool no_filename;
  bool silent_mode;
  bool file_pattern;
  bool only_matching;
};

static void print_match(const char *match_line, const char *filename, int line,
                        int count, const struct grep_opts *opt);
static char *extract_substring(const char *string, regex_t *compiled_pattern);
static int match_line(const char *line, regex_t *compiled_pattern,
                      int invert_match);
static char *get_pattern(struct grep_opts *opt, char *argv[const]);
static int compile_pattern(regex_t *compiled_pattern, const char *pattern,
                           const struct grep_opts *opt);
static int grep_process(char *argv[const], const char *pattern,
                        struct grep_opts *opt);
static void grep(const char *filename, FILE *fp, regex_t *compiled_pattern,
                 const struct grep_opts *opt);

static void usage(void);
static char *get_regerror(const int errorcode, regex_t *compiled_pattern);

static int get_options(int argc, char *argv[const], struct grep_opts *opt,
                       bool *requires_argument);
static int secure_usage(int argc, bool requires_argument);
static void validate_fname_printing(int argc, struct grep_opts *opt);
static int count_files(int argc);

static inline void init_grep_opts(struct grep_opts *opt) {
  opt->pattern = false;
  opt->ignore_case = false;
  opt->invert_match = false;
  opt->count = false;
  opt->files_with_matches = false;
  opt->line_number = false;
  opt->print_filename = false;
  opt->no_filename = false;
  opt->silent_mode = false;
  opt->file_pattern = false;
  opt->only_matching = false;
}

int main(int argc, char *argv[const]) {
  int retval = EXIT_SUCCESS;
  bool requires_argument = false;

  struct grep_opts opt;
  init_grep_opts(&opt);

  retval = get_options(argc, argv, &opt, &requires_argument);
  if (!retval) {
    retval = secure_usage(argc, requires_argument);
    if (!retval) {
      char *pattern = get_pattern(&opt, argv);
      if (!pattern)
        retval = ERR_GET_PATTERN;
      else {
        validate_fname_printing(argc, &opt);
        retval = grep_process(argv, pattern, &opt);
        free(pattern);
      }
    }
  }

  return retval;
}

static inline void usage() {
  printf(
      "Usage:\n"
      "grep [OPTION...] PATTERNS [FILE...]\n"
      "grep [OPTION...] -e PATTERNS ... [FILE...]\n"
      "grep [OPTION...] -f PATTERN_FILE ... [FILE...]\n");
}

static int count_files(int argc) {
  size_t fcounter = 1;
  for (int argind = optind; argind < argc - 1 && fcounter <= 1;
       ++argind, ++fcounter)
    ;

  return fcounter;
}

static void validate_fname_printing(int argc, struct grep_opts *opt) {
  if (count_files(argc) > 1) opt->print_filename = true;
}

static char *fread_pattern(const char *file_pattern) {
  char *pattern = NULL;
  FILE *fp = fopen(file_pattern, "r");
  if (fp) {
    size_t count_size = 0;
    int ch;
    while ((ch = getc(fp)) != EOF) {
      if (ch) ++count_size;
    }
    rewind(fp);
    pattern = malloc(count_size + 1);
    if (pattern) {
      count_size = 0;
      while ((ch = getc(fp)) != EOF) {
        if (ch && ch != '\n') pattern[count_size++] = ch;
        if (ch == '\n') pattern[count_size++] = '|';
      }
      pattern[--count_size] = '\0';
    }
  }

  return pattern;
}

static char *get_pattern(struct grep_opts *opt, char *argv[const]) {
  char *pattern = NULL;
  int argind = optind;
  --argind;
  if (opt->file_pattern) {
    pattern = fread_pattern(argv[argind]);
  } else {
    pattern = malloc(strlen(argv[argind]) + 1);
    if (pattern) strcpy(pattern, argv[argind]);
  }

  return pattern;
}

static int secure_usage(int argc, bool requires_argument) {
  int retval = 0;
  if (optind == argc) {
    usage();
    retval = ERR_MISSING_ARGUMENT;
  } else {
    if (!requires_argument) ++optind;
    if (optind == argc) {
      usage();
      retval = ERR_MISSING_ARGUMENT;
    }
  }

  return retval;
}

static int get_options(int argc, char *argv[const], struct grep_opts *opt,
                       bool *requires_argument) {
  int retval = 0;
  int c;
  while ((c = getopt(argc, argv, OPTS)) != -1) {
    switch (c) {
      case 'i':
        opt->ignore_case = true;
        break;
      case 'e':
        opt->pattern = true;
        *requires_argument = true;
        break;
      case 'f':
        opt->file_pattern = true;
        *requires_argument = true;
        break;
      case 'v':
        opt->invert_match = true;
        break;
      case 'c':
        opt->count = true;
        break;
      case 'l':
        opt->files_with_matches = true;
        break;
      case 'n':
        opt->line_number = true;
        break;
      case 'h':
        opt->no_filename = true;
        break;
      case 's':
        opt->silent_mode = true;
        break;
      case 'o':
        opt->only_matching = true;
        break;
      default:
      case ':':
        fprintf(stderr, "Option -%c requires an argument", optopt);
        retval = ERR_MISSING_ARGUMENT;
        break;
      case '?':
        fprintf(stderr, "Uknown option: %d\n", optopt);
        retval = ERR_UKNOWN_OPTION;
        break;
    }
  }

  return retval;
}

static char *get_regerror(const int errorcode, regex_t *compiled_pattern) {
  size_t len = regerror(errorcode, compiled_pattern, NULL, 0);
  char *buff = malloc(len);
  regerror(errorcode, compiled_pattern, buff, len);

  return buff;
}

static int compile_pattern(regex_t *compiled_pattern, const char *pattern,
                           const struct grep_opts *opt) {
  int flags = REG_EXTENDED;
  if (opt->ignore_case) flags |= REG_ICASE;

  int retval = regcomp(compiled_pattern, pattern, flags);
  if (retval) {
    const char *error = get_regerror(retval, compiled_pattern);
    fprintf(stderr, "pattern %s: %s\n", pattern, error);
  }

  return retval;
}

static void print_match(const char *match_line, const char *filename, int line,
                        int count, const struct grep_opts *opt) {
  if (opt->files_with_matches) {
    printf("%s\n", filename);
  } else {
    if (opt->print_filename && !opt->no_filename) printf("%s:", filename);
    if (opt->count)
      printf("%d\n", count);
    else if (opt->line_number && !opt->count)
      printf("%d:%s", line, match_line);
    else
      printf("%s", match_line);
  }
}

static int match_line(const char *line, regex_t *compiled_pattern,
                      int invert_match) {
  int retval = regexec(compiled_pattern, line, 0, NULL, 0);
  if (retval) {
    if (retval != REG_NOMATCH) {
      const char *error = get_regerror(retval, compiled_pattern);
      fprintf(stderr, "error: %s\n", error);
    }
    return invert_match;
  }
  return !invert_match;
}

static char *extract_substring(const char *string, regex_t *compiled_pattern) {
  char *substring = NULL;
  regmatch_t match;

  if ((regexec(compiled_pattern, string, 1, &match, 0)) == 0) {
    size_t len = match.rm_eo - match.rm_so;
    substring = malloc(len + 2);
    if (substring) {
      len = 0;
      for (int i = match.rm_so; i < match.rm_eo; i++)
        substring[len++] = string[i];
      substring[len++] = '\n';
      substring[len] = '\0';
    }
  }

  return substring;
}

static size_t count_match_in_line(const char *line, char *match) {
  size_t count = 0;

  match[strlen(match) - 1] = '\0';
  size_t match_len = strlen(match);

  while ((line = strstr(line, match))) {
    line += match_len;
    ++count;
  }

  match[match_len] = '\n';
  match[match_len + 1] = '\0';

  return count;
}

static void grep(const char *filename, FILE *fp, regex_t *compiled_pattern,
                 const struct grep_opts *opt) {
  char *buff = NULL;
  size_t size = 0;
  int gobble = 0;
  size_t count_match = 0;

  for (size_t line = 1; getline(&buff, &size, fp) != -1; ++line) {
    if (match_line(buff, compiled_pattern, opt->invert_match)) {
      if (opt->files_with_matches) {
        if (!gobble) {
          print_match(buff, filename, line, count_match, opt);
          gobble = true;
        } else {
          continue;
        }
      } else if (opt->count) {
        ++count_match;
        continue;
      } else {
        if (opt->only_matching) {
          char *substring = extract_substring(buff, compiled_pattern);
          size_t match_in_line = count_match_in_line(buff, substring);
          for (; match_in_line; --match_in_line) {
            print_match(substring, filename, line, count_match, opt);
          }
          free(substring);
        } else {
          print_match(buff, filename, line, count_match, opt);
        }
      }
    }
  }
  if (opt->count && !opt->files_with_matches)
    print_match(buff, filename, 0, count_match, opt);

  free(buff);
}

static int grep_process(char *argv[const], const char *pattern,
                        struct grep_opts *opt) {
  int retval = 0;
  regex_t regex;
  retval = compile_pattern(&regex, pattern, opt);
  if (!retval) {
    for (; argv[optind]; ++optind) {
      FILE *fp = fopen(argv[optind], "r");
      retval = fp ? 0 : ERR_NO_FILE;
      if (retval) {
        if (opt->silent_mode) continue;
        fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
      } else {
        grep(argv[optind], fp, &regex, opt);
        fclose(fp);
      }
    }
  }
  regfree(&regex);

  return retval;
}
