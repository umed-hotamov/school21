#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024

enum errors {
  ERR_NO_OPT = 1,
  ERR_NO_FILE,
  ERR_UKNOWN_OPTION,
  ERR_MISSING_ARGUMENT
};

struct cat_opts {
  bool nonblank;
  bool show_end;
  bool number;
  bool squeeze_blank;
  bool show_tab;
  bool show_nonprinting;
};

static void cat(FILE *fp, const struct cat_opts *opt);
static int cat_process(char **argv, const struct cat_opts *opt);

static int opt_scanner(int argc, char **argv, struct cat_opts *opt,
                       int *has_opt);
static inline void init_opt(struct cat_opts *opt);

static void usage(void);
static int secure_usage(int argc, int has_opt, int uknown_opt);

int main(int argc, char *argv[const]) {
  struct cat_opts opt;
  init_opt(&opt);

  int has_opt = 0;
  int retval = opt_scanner(argc, argv, &opt, &has_opt);
  retval = secure_usage(argc, has_opt, retval);
  if (!retval) retval = cat_process(argv, &opt);

  return retval;
}

static inline void usage(void) { printf("Usage: cat [OPTION]... [FILE]...\n"); }

static int secure_usage(int argc, int has_opt, int uknown_opt) {
  int retval = 0;
  if (uknown_opt) {
    usage();
    retval = ERR_UKNOWN_OPTION;
  } else {
    if (argc == 1) {
      usage();
      retval = ERR_MISSING_ARGUMENT;
    } else {
      if (has_opt) {
        --argc;
        while (has_opt--) --argc;
        if (!argc) {
          usage();
          retval = ERR_MISSING_ARGUMENT;
        }
      }
    }
  }

  return retval;
}

static int opt_scanner(int argc, char **argv, struct cat_opts *opt,
                       int *has_opt) {
  int retval = 0;

  int optcount = 0;
  for (; argc > 1; --argc, ++argv) {
    if (argv[1][0] == '-') {
      switch (argv[1][1]) {
        case 'b':
          opt->nonblank = true;
          ++optcount;
          break;
        case 'e':
          opt->show_end = true;
          opt->show_nonprinting = true;
          ++optcount;
          break;
        case 'E':
          opt->show_end = true;
          ++optcount;
          break;
        case 'n':
          opt->number = true;
          ++optcount;
          break;
        case 't':
          opt->show_tab = true;
          opt->show_nonprinting = true;
          ++optcount;
          break;
        case 'T':
          opt->show_tab = true;
          ++optcount;
          break;
        case 's':
          opt->squeeze_blank = true;
          ++optcount;
          break;
        case 'v':
          opt->show_nonprinting = true;
          ++optcount;
          break;
        case '-':
          if (!strcmp(argv[1], "--number-nonblank")) {
            ++optcount;
            opt->nonblank = true;
          } else if (!strcmp(argv[1], "--number")) {
            ++optcount;
            opt->number = true;
          } else if (!strcmp(argv[1], "--squeeze-blank")) {
            ++optcount;
            opt->squeeze_blank = true;
          }
          break;
        default:
          fprintf(stderr, "Uknown option -%c\n", argv[1][1]);
          retval = ERR_UKNOWN_OPTION;
      }
    }
  }
  *has_opt = optcount;

  return retval;
}

static inline void init_opt(struct cat_opts *opt) {
  opt->nonblank = false;
  opt->show_end = false;
  opt->number = false;
  opt->squeeze_blank = false;
  opt->show_tab = false;
  opt->show_nonprinting = false;
}

static void cat(FILE *fp, const struct cat_opts *opt) {
  static int line;
  int gobble = 0;
  int ch, prev;

  for (prev = '\n'; (ch = getc(fp)) != EOF; prev = ch) {
    if (prev == '\n') {
      if (opt->squeeze_blank) {
        if (ch == '\n') {
          if (gobble) continue;
          gobble = 1;
        } else
          gobble = 0;
      }
      if (opt->number) {
        if (!opt->nonblank || ch != '\n')
          fprintf(stdout, "%6d\t", ++line);
        else if (opt->show_end && !opt->nonblank)
          fprintf(stdout, "%6s\t", "");
      } else if (opt->nonblank && ch != '\n')
        fprintf(stdout, "%6d\t", ++line);
    }
    if (ch == '\n') {
      if (opt->show_end)
        if (putchar('$') == EOF) break;
    } else if (ch == '\t') {
      if (opt->show_tab) {
        if (putchar('^') == EOF || putchar('I') == EOF) break;
        continue;
      }
    } else if (opt->show_nonprinting) {
      if (!isascii(ch)) {
        if (putchar('M') == EOF || putchar('-') == EOF) break;
        ch = toascii(ch);
      }
      if (iscntrl(ch)) {
        if (putchar('^') == EOF ||
            putchar(ch == '\177' ? '?' : ch | 0100) == EOF)
          break;
        continue;
      }
    }
    if (putchar(ch) == EOF) break;
  }
}

static int cat_process(char **argv, const struct cat_opts *opt) {
  int retval;
  FILE *fp;
  do {
    argv++;
    if (*argv && **argv != '-') {
      if ((fp = fopen(*argv, "r")) == NULL) {
        fprintf(stderr, "cat: %s: No such file or directory\n", *argv);
        retval = ERR_NO_FILE;
        continue;
      } else {
        cat(fp, opt);
        fclose(fp);
        retval = EXIT_SUCCESS;
      }
    }

  } while (*argv);

  return retval;
}
