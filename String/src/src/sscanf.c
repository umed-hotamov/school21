#include "s21_string.h"

static int s21_vsscanf(const char *str, const char *fmt, va_list args);

int s21_sscanf(const char *str, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int count = s21_vsscanf(str, fmt, args);
  va_end(args);

  return count;
}

static char *strint(const char *str, int *value, int width, int base) {
  char *lookup = (char *)str;
  int tmp = 0;
  while (!isdigit(*lookup) && *lookup)
    ++lookup;

  if (!*lookup)
    return NULL;

  int is_negative = 0;
  if (*lookup != *str) {
    if (*(lookup - 1) == '-') {
      is_negative = 1;
    }
  }

  do {
    tmp = tmp * base + (*lookup - '0');
    ++lookup;
    width = width > 0 ? width - 1 : -1;
    if (!width)
      break;

  } while (isdigit(*lookup));

  if (is_negative)
    tmp = -tmp;

  *value = tmp;

  return lookup;
}

static char *strc(const char *str, char *ch) {
  char *lookup = (char *)str;
  int c;
  if (isascii(*lookup))
    c = *lookup;

  *ch = c;

  return ++lookup;
}

static char *struint(const char *str, unsigned int *value, int width,
                     int base) {
  char *lookup = (char *)str;
  int num;

  lookup = strint(lookup, &num, width, base);
  *value = (unsigned int)num;

  return lookup;
}

static char *strhex(const char *str, unsigned int *value, int width) {
  char *lookup = (char *)str;

  while (!isxdigit(*lookup) && *lookup)
    ++lookup;

  if (!*lookup)
    return NULL;

  int len = 0;
  char ch;
  do {
    ++lookup;
    ++len;
    width = width > 0 ? width - 1 : -1;
    if (!width)
      break;
  } while (isxdigit(*lookup));

  if (!isxdigit(*lookup))
    --lookup;

  char *end = lookup;
  uintmax_t hex = 0;
  unsigned int digit = 1;
  int ind = 0;

  while (len--) {
    switch (*end) {
    case 'a':
    case 'A':
      ind = 10;
      break;
    case 'b':
    case 'B':
      ind = 11;
      break;
    case 'c':
    case 'C':
      ind = 12;
      break;
    case 'd':
    case 'D':
      ind = 13;
      break;
    case 'e':
    case 'E':
      ind = 14;
      break;
    case 'f':
    case 'F':
      ind = 15;
      break;
    default:
      ind = *end - 0x30;
    }
    hex += ind * digit;
    digit *= 16;
    --end;
  }

  *value = hex;

  return ++lookup;
}

static char *strfloat(const char *str, float *value, int width) {
  char *lookup = (char *)str;
  int tmp = 0;
  int mantissa = 0;

  int is_negative = 0;
  if (*lookup == '-') {
    is_negative = 1;
    ++lookup;
  }

  while (isdigit(*lookup) && *lookup) {
    tmp = tmp * 10 + (*lookup - '0');
    mantissa += 10;
    ++lookup;
    width = width > 0 ? width - 1 : -1;
    if (!width)
      break;
  }

  if (*lookup == '.') {
    ++lookup;
    if (!isdigit(*lookup))
      mantissa = 1;

    while (isdigit(*lookup) && *lookup) {
      tmp = tmp * 10 + (*lookup - '0');
      ++lookup;
      width = width > 0 ? width - 1 : -1;
      if (!width)
        break;
    }
  } else {
    mantissa = 1;
  }
  if (tmp) {
    if (is_negative)
      tmp = -tmp;
    *value = (float)tmp / mantissa;
  } else {
    *value = 0;
  }

  return lookup;
}

static char *strs(const char *str, char *strarg, int width) {
  return s21_strncpy(strarg, str, width ? width : s21_strlen(str));
}

static int s21_vsscanf(const char *str, const char *fmt, va_list args) {
  char *tmp_str = (char *)str;
  s21_size_t count = 0;
  while (*fmt && *tmp_str) {
    while (*fmt != '%' && *fmt) {
      ++fmt;
    }
    if (!*fmt)
      break;
    ++fmt;

    int base = 10;
    int width = 0;

    if (isdigit(*fmt)) {
      do {
        width = width * base + (*fmt - '0');
        ++fmt;
      } while (isdigit(*fmt));
    }
    if (*fmt == '*')
      continue;

    switch (*fmt) {
    case 'd':
      tmp_str = strint(tmp_str, va_arg(args, int *), width, base);
      ++fmt;
      ++count;
      break;
    case 'u':
    case 'o':
      if (*fmt == 'o') {
        base = 8;
      }
      tmp_str = struint(tmp_str, va_arg(args, unsigned int *), width, base);
      ++fmt;
      ++count;
      break;
    case 'x':
    case 'X':
      tmp_str = strhex(tmp_str, va_arg(args, unsigned int *), width);
      ++fmt;
      ++count;
      break;
    case 'c':
      tmp_str = strc(tmp_str, va_arg(args, char *));
      ++fmt;
      ++count;
      break;
    case 's':
      tmp_str = strs(tmp_str, va_arg(args, char *), width);
      ++fmt;
      ++count;
      break;
    case 'f':
      tmp_str = strfloat(tmp_str, va_arg(args, float *), width);
      ++fmt;
      ++count;
      break;
    }
  }

  return count;
}
