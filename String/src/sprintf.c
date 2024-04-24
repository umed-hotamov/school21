#include <limits.h>
#include <stdio.h>

#include "s21_string.h"

struct print_opts {
  char padding_sign;
  int width;
  int precision;
  int base;
  unsigned has_precision : 1;
  unsigned is_plus : 1;
  unsigned is_left_padding : 1;
  unsigned is_space : 1;
  unsigned is_prefix : 1;
  unsigned is_upper_prefix : 1;
};

#define put(_buffer, _index, _c) (_buffer)[(_index)] = _c

static int s21_vsprintf(char *buffer, const char *fmt, va_list args);

int s21_sprintf(char *buffer, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int count = s21_vsprintf(buffer, fmt, args);
  va_end(args);

  return count;
}

static void put_signed(char *buffer, s21_size_t *size, intmax_t number,
                       struct print_opts *opts) {
  char number_buffer[MAX_NUMBER_SIZE];
  s21_size_t tmp_size = *size;

  s21_itoa(number, number_buffer, opts->base);
  s21_size_t num_len = s21_strlen(number_buffer);

  opts->width = opts->width > num_len ? opts->width - num_len : 0;
  if (number >= 0 && (opts->is_plus || opts->is_space)) {
    --opts->width;
  }

  if (!opts->is_left_padding && opts->width > 0) {
    while (opts->width--) {
      put(buffer, tmp_size++, opts->padding_sign);
    }
  }
  if (number >= 0 && (opts->is_plus || opts->is_space)) {
    put(buffer, tmp_size++, opts->is_plus ? '+' : ' ');
  }

  for (int i = 0; i < num_len; ++i) {
    put(buffer, tmp_size++, number_buffer[i]);
  }

  if (opts->is_left_padding && opts->width > 0) {
    if (opts->padding_sign == '0') opts->padding_sign = ' ';
    while (opts->width--) {
      put(buffer, tmp_size++, opts->padding_sign);
    }
  }

  *size = tmp_size;
}
static void put_unsigned(char *buffer, s21_size_t *size, uintmax_t number,
                         struct print_opts *opts) {
  char number_buffer[MAX_NUMBER_SIZE];
  s21_size_t tmp_size = *size;

  s21_itoa(number, number_buffer, opts->base);
  s21_size_t num_len = s21_strlen(number_buffer);
  opts->width = opts->width > num_len ? opts->width - num_len : 0;
  if (opts->is_prefix) {
    if (opts->base == 8) {
      --opts->width;
    } else if (opts->base == 16) {
      opts->width -= 2;
    }
  }
  if (opts->base == 16 && opts->padding_sign == '0') {
    put(buffer, tmp_size++, '0');
    put(buffer, tmp_size++, opts->is_upper_prefix ? 'X' : 'x');
  }
  if (!opts->is_left_padding && opts->width > 0)
    for (; opts->width; --opts->width) {
      put(buffer, tmp_size++, opts->padding_sign);
    }
  if (opts->is_prefix) {
    if (opts->base == 8) {
      put(buffer, tmp_size++, '0');
    } else if (opts->base == 16 && opts->padding_sign != '0') {
      put(buffer, tmp_size++, '0');
      put(buffer, tmp_size++, opts->is_upper_prefix ? 'X' : 'x');
    }
  }

  for (int i = 0; i < num_len; ++i) {
    put(buffer, tmp_size++,
        opts->is_upper_prefix ? toupper(number_buffer[i]) : number_buffer[i]);
  }
  if (opts->is_left_padding && opts->width > 0) {
    if (opts->padding_sign == '0') opts->padding_sign = ' ';
    while (opts->width--) {
      put(buffer, tmp_size++, opts->padding_sign);
    }
  }
  *size = tmp_size;
}

static void put_string(char *buffer, s21_size_t *size, const char *string,
                       struct print_opts *opts) {
  s21_size_t tmp_size = *size;
  s21_size_t len = s21_strlen(string);

  opts->width = opts->width > len ? opts->width - len : 0;
  if (!opts->is_left_padding) {
    for (; opts->width; --opts->width) {
      put(buffer, tmp_size++, opts->padding_sign);
    }
  }
  if (opts->precision) {
    opts->precision = opts->precision > len ? len : opts->precision;
  } else {
    opts->precision = len;
  }
  while (opts->precision--) {
    put(buffer, tmp_size++, *string++);
  }
  if (opts->is_left_padding) {
    while (opts->width--) {
      put(buffer, tmp_size++, opts->padding_sign);
    }
  }
  *size = tmp_size;
}

static void put_double(char *buffer, s21_size_t *size, double number,
                       struct print_opts *opts) {
  char number_buffer[MAX_NUMBER_SIZE];
  s21_size_t tmp_size = *size;

  int precision = DEFAULT_FPREC_WIDTH;

  if (opts->precision) precision = opts->precision;

  if (opts->has_precision && !opts->precision) precision = 0;

  s21_ftoa(number, number_buffer, precision);
  s21_size_t num_len = s21_strlen(number_buffer);

  opts->width = opts->width > num_len ? opts->width - num_len : 0;
  if (number >= 0 && (opts->is_plus || opts->is_space)) {
    --opts->width;
  }
  if (!opts->is_left_padding && opts->width > 0) {
    while (opts->width--) {
      put(buffer, tmp_size++, opts->padding_sign);
    }
  }
  if (number >= 0 && (opts->is_plus || opts->is_space)) {
    put(buffer, tmp_size++, opts->is_plus ? '+' : ' ');
  }
  for (s21_size_t i = 0; i < num_len; ++i) {
    put(buffer, tmp_size++, number_buffer[i]);
  }
  if (opts->is_left_padding && opts->width > 0) {
    if (opts->padding_sign == '0') opts->padding_sign = ' ';
    while (opts->width--) {
      put(buffer, tmp_size++, opts->padding_sign);
    }
  }
  *size = tmp_size;
}

static int s21_vsprintf(char *buffer, const char *fmt, va_list args) {
  // format: %[flags][width][.precision][length]
  /* Specifiers: c, d, f, s, u, %, x, X, o, p */
  /* Flags: -, +, (space) | #, 0 */
  /* Width description: (number) */
  /* Precision description: .(number) */
  /* Length description: h, l, L */

  s21_size_t size = 0;
  while (*fmt) {
    while (*fmt != '%' && *fmt) {
      put(buffer, size++, *fmt++);
    }
    if (!*fmt) break;
    ++fmt;

    struct print_opts opts = {0};
    opts.padding_sign = ' ';
    opts.base = 10;

    for (;; ++fmt) {
      if (*fmt == '+') {
        opts.is_plus = 1;
        continue;
      } else if (*fmt == '-') {
        opts.is_left_padding = 1;
        continue;
      } else if (*fmt == ' ') {
        opts.is_space = 1;
        continue;
      } else if (*fmt == '#') {
        opts.is_prefix = 1;
        continue;
      } else if (*fmt == '0') {
        opts.padding_sign = '0';
        continue;
      }
      break;
    }
    if (isdigit(*fmt)) {
      do {
        opts.width = opts.width * opts.base + (*fmt - '0');
        ++fmt;
      } while (isdigit(*fmt));
    } else if (*fmt == '*') {
      opts.width = va_arg(args, int);
      ++fmt;
    }
    if (*fmt == '.') {
      opts.has_precision = 1;
      ++fmt;
      if (isdigit(*fmt)) {
        do {
          opts.precision = opts.precision * opts.base + (*fmt - '0');
          ++fmt;
        } while (isdigit(*fmt));
      } else if (*fmt == '*') {
        opts.precision = va_arg(args, int);
        ++fmt;
      }
    }
    switch (*fmt) {
      case 'l':
      case 'L':
      case 'h':
        ++fmt;
        switch (*fmt) {
          case 'u':
          case 'o':
          case 'x':
          case 'X':
            if (*fmt == 'o') opts.base = 8;
            if (*fmt == 'x' || *fmt == 'X' || *fmt == 'p') opts.base = 16;
            if (*fmt == 'X') opts.is_upper_prefix = 1;
            put_unsigned(buffer, &size, va_arg(args, unsigned int), &opts);
            ++fmt;
            break;
          case 'd':
            put_signed(buffer, &size, va_arg(args, int), &opts);
            ++fmt;
            break;
          case 'f':
            put_double(buffer, &size, va_arg(args, double), &opts);
            ++fmt;
            break;
        }
        break;
      case 'u':
      case 'o':
      case 'x':
      case 'X':
        if (*fmt == 'o') opts.base = 8;
        if (*fmt == 'x' || *fmt == 'X' || *fmt == 'p') opts.base = 16;
        if (*fmt == 'X') opts.is_upper_prefix = 1;
        put_unsigned(buffer, &size, va_arg(args, unsigned int), &opts);
        ++fmt;
        break;
      case 'p':
        opts.is_prefix = 1;
        opts.base = 16;
        put_unsigned(buffer, &size, (uintptr_t)va_arg(args, void *), &opts);
        ++fmt;
        break;
      case 'd':
        put_signed(buffer, &size, va_arg(args, int), &opts);
        ++fmt;
        break;
      case 'c':
        put(buffer, size++, (char)va_arg(args, int));
        ++fmt;
        break;
      case 's':
        put_string(buffer, &size, va_arg(args, char *), &opts);
        ++fmt;
        break;
      case 'f':
        put_double(buffer, &size, va_arg(args, double), &opts);
        ++fmt;
        break;
      case '%':
        put(buffer, size++, '%');
        ++fmt;
        break;
    }
  }
  buffer[size] = '\0';

  return size;
}

/* int main(void) { */
/*     char buf[200]; */
/*     char buf1[200]; */

/*     s21_sprintf(buf, "%+-10d %-20s %+.2f abc", 15, "abcdefg", 25.5); */
/*     sprintf(buf1, "%+-10d %-20s %+.2f abc", 15, "abcdefg", 25.5); */

/*     printf("buff  - %s\n", buf); */
/*     printf("buff1 - %s\n", buf1); */
/* } */
