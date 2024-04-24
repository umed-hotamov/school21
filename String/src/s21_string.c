#include "s21_string.h"

#include <stdio.h>
#include <string.h>

void *s21_memchr(const void *str, int c, s21_size_t n) {
  const unsigned char *p = str;
  while (n-- && *p) {
    if (*p++ == (unsigned char)c) return (void *)p - 1;
  }

  return s21_NULL;
}

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  const unsigned char *p1 = str1;
  const unsigned char *p2 = str2;

  int diff = 0;
  while (n-- && *p1 && *p2) {
    diff = *p1 - *p2;
    if (*p1++ != *p2++) break;
  }

  return diff;
}

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  const unsigned char *s = src;
  unsigned char *d = dest;

  while (n--) {
    *d++ = *s++;
  }

  return dest;
}

void *s21_memset(void *str, int c, s21_size_t n) {
  unsigned char *s = str;
  while (n--) *s++ = c;

  return str;
}

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  char *tmp = dest;
  while ((*dest++ = *src++) != '\0') {
    if (!--n) {
      *dest = '\0';
      break;
    }
  }

  return tmp;
}

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *tmp = dest;
  if (n) {
    while (*dest) ++dest;
    s21_strncpy(dest, src, n);
  }

  return tmp;
}

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  unsigned char c1, c2;

  while (n--) {
    c1 = *str1++;
    c2 = *str2++;

    if (c1 != c2) {
      return c1 - c2;
    }
    if (!c1) break;
  }

  return 0;
}

s21_size_t s21_strlen(const char *str) {
  const char *tmp = str;
  for (; *str; ++str)
    ;

  return str - tmp;
}

char *s21_strchr(const char *str, int ch) {
  for (; *str != (char)ch; ++str) {
    if (!*str) return s21_NULL;
  }

  return (char *)str;
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  const char *p;

  for (p = str1; *p; ++p) {
    if (strchr(str2, *p)) break;
  }

  return p - str1;
}

char *s21_strerror(int errnum) {
  char *error = s21_NULL;
  static char result[42];
  char digit[12] = "";
  char *s21_error[] = s21_errors;
  if (errnum <= s21_N_error && errnum >= 0) {
    s21_strncpy(result, s21_error[errnum], s21_strlen(s21_error[errnum]));
    error = result;
  } else {
#if defined(__APPLE__) && defined(__MACH__)
    char *msg = "Unknown error: ";
    s21_strncpy(result, msg, s21_strlen(msg));
#elif defined(__linux__)
    char *msg = "Unknown error ";
    s21_strncpy(result, msg, s21_strlen(msg));
#endif
    if (errnum < 0) {
      s21_strncat(digit, "-", 1);
      errnum *= -1;
    }
    s21_itoa(errnum, &digit[s21_strlen(digit)], 10);
    s21_strncat(result, digit, s21_strlen(digit));
    error = result;
  }
  return error;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  char *scan = (char *)str1;
  char ch;
  while ((ch = *scan++)) {
    for (const char *search = str2; *search; ++search) {
      if (ch == *search) {
        return --scan;
      }
    }
  }

  return s21_NULL;
}

char *s21_strrchr(const char *str, int c) {
  char *end = (char *)str + s21_strlen(str);
  do {
    if (*end == c) return end;
  } while (*end-- != *str);

  return s21_NULL;
}

char *s21_strstr(const char *haystack, const char *needle) {
  s21_size_t test_len = s21_strlen(needle);
  if (!test_len) return (char *)haystack;

  char *start = s21_strchr(haystack, *needle);
  if (start) {
    do {
      if (s21_strncmp(start, needle, test_len) == 0) {
        return start;
      }
    } while ((start = s21_strchr(start, *needle)));
  }

  return s21_NULL;
}

static int is_delim(char c, char *delim) {
  while (*delim != '\0') {
    if (c == *delim) return 1;
    delim++;
  }
  return 0;
}

char *s21_strtok(char *src, char *delim) {
  static char *backup_string;
  if (!src) {
    src = backup_string;
  }
  if (!src) {
    return s21_NULL;
  }
  for (;;) {
    if (is_delim(*src, delim)) {
      src++;
      continue;
    }
    if (*src == '\0') {
      return s21_NULL;
    }
    break;
  }
  char *ret = src;
  for (;;) {
    if (*src == '\0') {
      backup_string = src;
      return ret;
    }
    if (is_delim(*src, delim)) {
      *src = '\0';
      backup_string = src + 1;
      return ret;
    }
    src++;
  }
}

char *to_upper(const char *str) {
  if (!str) return s21_NULL;

  s21_size_t len = s21_strlen(str);
  char *upper_str = malloc(len + 1);
  s21_size_t new_len = 0;
  if (upper_str) {
    while (*str) upper_str[new_len++] = toupper(*str++);
  }
  upper_str[new_len] = '\0';

  return upper_str;
}

char *to_lower(const char *str) {
  if (!str) return s21_NULL;

  s21_size_t len = s21_strlen(str);
  char *lower_str = malloc(len + 1);
  s21_size_t new_len = 0;
  if (lower_str) {
    while (*str) lower_str[new_len++] = tolower(*str++);
  }
  lower_str[new_len] = '\0';

  return lower_str;
}

char *insert(const char *src, const char *str, s21_size_t start_index) {
  if (!src || !str) return s21_NULL;

  s21_size_t src_len = s21_strlen(src);
  s21_size_t str_len = s21_strlen(str);

  s21_size_t new_len = 0;
  char *new_str = malloc(src_len + str_len + 1);
  if (new_str) {
    while (new_len != start_index) new_str[new_len++] = *src++;
    while (*str) new_str[new_len++] = *str++;
    while (*src) new_str[new_len++] = *src++;
  }
  new_str[new_len] = '\0';

  return new_str;
}

static void do_trim(char *trimmed, const char *src, int trim_ch) {
  if (trim_ch) {
    s21_size_t count = 0;
    for (; *src; ++src) {
      if (*src != trim_ch) {
        trimmed[count++] = *src;
      }
    }
  }
}

char *trim(const char *src, const char *trim_chars) {
  if (!src || !trim_chars) return s21_NULL;

  s21_size_t occur = 0;
  char *scan = s21_strpbrk(src, trim_chars);
  if (scan) {
    do {
      ++occur;
      ++scan;
    } while ((scan = s21_strpbrk(scan, trim_chars)));
  }
  s21_size_t len = s21_strlen(src) - occur;
  char *trimmed = s21_NULL;
  if (occur) {
    trimmed = malloc(len + 1);
    if (trimmed) {
      while (*trim_chars) do_trim(trimmed, src, *trim_chars++);
    }
  }
  trimmed[len] = '\0';

  return trimmed;
}

static void reverse_buffer(char *buffer) {
  s21_size_t len = s21_strlen(buffer);

  for (s21_size_t i = 0; i < len / 2; ++i) {
    char temp = buffer[len - i - 1];
    buffer[len - i - 1] = buffer[i];
    buffer[i] = temp;
  }
}

char *s21_itoa(intmax_t number, char *number_buffer, int base) {
  s21_size_t counter = 0;
  if (number < 0) {
    number_buffer[counter++] = '-';
    number = -number;
  }
  while (number > 0) {
    int digit = number % base;
    number_buffer[counter++] = "0123456789abcdef"[digit];
    number /= base;
  }
  number_buffer[counter] = '\0';
  reverse_buffer(number_buffer);

  return number_buffer;
}

char *s21_ftoa(double number, char *buffer, int precision) {
  const double rounders[MAX_PRECISION + 1] = {
      0.5,       0.05,       0.005,       0.0005,       0.00005,      0.000005,
      0.0000005, 0.00000005, 0.000000005, 0.0000000005, 0.00000000005};

  if (precision > MAX_PRECISION) precision = MAX_PRECISION;

  if (number < 0) {
    s21_strncpy(buffer, "-", 1);
    number = -number;
  }
  if (precision) number += rounders[precision];

  int int_part = number;
  double prec_part = number - int_part;

  char intp_buffer[MAX_NUMBER_SIZE];
  s21_itoa(int_part, intp_buffer, 10);
  s21_strncpy(buffer, intp_buffer, s21_strlen(intp_buffer));

  s21_size_t curr = s21_strlen(buffer);
  if (precision) buffer[curr++] = '.';
  while (precision--) {
    prec_part *= 10;
    char c = prec_part;
    buffer[curr++] = c + '0';
    prec_part -= c;
  }
  buffer[curr] = '\0';

  return buffer;
}
