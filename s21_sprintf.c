#include "s21_sprintf.h"

#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

#include "s21_datatypes.h"
#include "s21_string.h"
#include "s21_utils.h"
#define BUFF_SIZE 512

int s21_sprintf(char *str, const char *format, ...) {
  specifications specs = {0};
  char *str0 = str;
  va_list va;

  va_start(va, format);
  while (*format) {
    if (*format != '%') {
      *str++ = *format++;
      continue;
    } else {
      format++;
      specs = (specifications){0};
    }

    format = scan_flags(format, &specs);
    format = scan_width(format, &specs, &va);
    format = scan_precision(format, &specs, &va);
    format = scan_length(format, &specs);

    specs.specifier = *format;
    format++;

    char buff[BUFF_SIZE] = "";
    parse_value(&specs, buff, &va);
    for (int i = 0; buff[i]; i++, str++) {
      *str = buff[i];
    }

    if (specs.specifier == 'n') {
      int *ret = va_arg(va, int *);
      *ret = str - str0;
    }
  }
  va_end(va);

  *str = '\0';
  return str - str0;
}

const char *scan_flags(const char *format, specifications *specs) {
  while (s21_ischarset(*format, 5, '-', '+', ' ', '0', '#')) {
    switch (*format) {
      case '0':
        specs->zero = true;
        break;
      case '-':
        specs->minus = true;
        break;
      case '+':
        specs->plus = true;
        break;
      case ' ':
        specs->space = true;
        break;
      case '#':
        specs->hash = true;
        break;
    }
    format++;
  }

  return format;
}

const char *scan_width(const char *format, specifications *specs, va_list *va) {
  if (*format == '*') {
    format++;
    specs->width = va_arg(*va, int);
  } else if (isdigit(*format)) {
    char tmp[BUFF_SIZE] = "";
    for (int i = 0; isdigit(*format); i++, format++) {
      tmp[i] = *format;
    }
    specs->width = s21_atoi(tmp);
  }

  return format;
}

const char *scan_precision(const char *format, specifications *specs,
                           va_list *va) {
  if (*format == '.') {
    specs->is_precision_set = true;
    format++;
  }
  if (*format == '*') {
    format++;
    specs->precision = va_arg(*va, int);
  }
  if (isdigit(*format)) {
    char tmp[BUFF_SIZE] = "";

    for (int i = 0; isdigit(*format); i++, format++) {
      tmp[i] = *format;
    }
    specs->precision = s21_atoi(tmp);
  }

  return format;
}

const char *scan_length(const char *format, specifications *specs) {
  switch (*format) {
    case 'h':
      specs->length = 'h';
      format++;
      break;
    case 'l':
      specs->length = 'l';
      format++;
      break;
    case 'L':
      specs->length = 'L';
      format++;
      break;
  }

  return format;
}

void parse_value(specifications *specs, char *buff, va_list *va) {
  switch (specs->specifier) {
    case 'd':
    case 'i':
      parse_int(specs, buff, va);
      break;
    case 'u':
      parse_uint(specs, buff, va);
      break;
    case 'o':
      parse_octal(specs, buff, va);
      break;
    case 'x':
    case 'X':
      parse_hex(specs, buff, va);
      break;
    case '%':
      buff[0] = '%';
      break;
    case 'c':
      parse_char(specs, buff, va);
      break;
    case 's':
      parse_string(specs, buff, va);
      break;
    case 'p':
      parse_pointer(specs, buff, va);
      break;
    case 'f':
      parse_float(specs, buff, va);
      break;
    case 'e':
    case 'E':
      parse_mantissa(specs, buff, va);
      break;
    case 'g':
    case 'G':
      parse_float_g_G(specs, buff, va);
      break;
  }
  if (s21_ischarset(specs->specifier, 3, 'E', 'G', 'X')) {
    to_upper(buff);
  }
}

void parse_int(specifications *specs, char *buff, va_list *va) {
  int64_t val = va_arg(*va, int64_t);
  switch (specs->length) {
    case 0:
      val = (int32_t)val;
      break;
    case 'h':
      val = (int16_t)val;
      break;
  }

  s21_itoa(val, buff, 10);
  format_precision(buff, specs);
  format_flags(buff, specs);
}

void format_precision(char *buff, specifications *specs) {
  char tmp[BUFF_SIZE] = "";

  int sign = 0;
  int len = s21_strlen(buff);

  if (buff[0] == '-') {
    tmp[0] = '-';
    len--;
    sign = 1;
  }

  if (specs->precision > len) {
    int idx;
    for (idx = sign; idx < specs->precision - len + sign; idx++) {
      tmp[idx] = '0';
    }

    for (int i = sign; buff[i]; i++, idx++) {
      tmp[idx] = buff[i];
    }

    s21_strcpy(buff, tmp);
  }

  if (specs->is_precision_set && specs->precision == 0 &&
      is_integer_specifier(specs->specifier) && buff[0] == '0') {
    buff[0] = '\0';
  }
}

bool is_integer_specifier(char c) {
  return s21_ischarset(c, 6, 'd', 'i', 'o', 'u', 'x', 'X');
}

void format_flags(char *buff, specifications *specs) {
  char tmp[BUFF_SIZE] = "";
  if (specs->plus && specs->specifier != 'u') {
    tmp[0] = buff[0] == '-' ? buff[0] : '+';
    s21_strcpy(tmp + 1, buff[0] == '-' ? buff + 1 : buff);
    s21_strcpy(buff, tmp);
  } else if (specs->space && buff[0] != '-' && specs->specifier != 'u') {
    tmp[0] = ' ';
    s21_strcpy(tmp + 1, buff);
    s21_strcpy(buff, tmp);
  }
  if (specs->width > (int)s21_strlen(buff)) {
    int idx = specs->width - s21_strlen(buff);
    if (!specs->minus) {
      s21_memset(tmp, specs->zero ? '0' : ' ', idx);
      s21_strcpy(tmp + idx, buff);
    } else {
      s21_strcpy(tmp, buff);
      s21_memset(tmp + s21_strlen(tmp), ' ', idx);
    }
    s21_strcpy(buff, tmp);
  }
}

void parse_uint(specifications *specs, char *buff, va_list *va) {
  uint64_t val = va_arg(*va, uint64_t);
  switch (specs->length) {
    case 'h':
      val = (uint16_t)val;
      break;
    case 'l':
      val = (uint64_t)val;
      break;
    case 0:
      val = (uint32_t)val;
  }

  s21_uitoa(val, buff, 10);
  format_precision(buff, specs);
  format_flags(buff, specs);
}

void parse_octal(specifications *specs, char *buff, va_list *va) {
  buff[0] = '0';
  s21_itoa(va_arg(*va, int64_t), buff + specs->hash, 8);
  format_precision(buff, specs);
  format_flags(buff, specs);
}

void to_upper(char *str) {
  while (*str) {
    if (*str >= 'a' && *str <= 'z') {
      *str = *str - 'a' + 'A';
    }
    str++;
  }
}

bool is_all_zeroes(char *buff) {
  for (int i = 0; buff[i]; i++)
    if (buff[i] != '0') {
      return false;
    }
  return true;
}

void parse_hex(specifications *specs, char *buff, va_list *va) {
  uint64_t val = va_arg(*va, uint64_t);
  switch (specs->length) {
    case 0:
      val = (uint32_t)val;
      break;
    case 'h':
      val = (uint16_t)val;
      break;
    case 'l':
      val = (uint64_t)val;
      break;
  }

  s21_uitoa(val, buff, 16);
  format_precision(buff, specs);
  if (specs->hash) {
    prepend_ox(buff, specs);
  }
  format_flags(buff, specs);
}

void prepend_ox(char *buff, specifications *specs) {
  if (!is_all_zeroes(buff) || specs->specifier == 'p') {
    s21_memmove(buff + 2, buff, s21_strlen(buff));
    buff[0] = '0';
    buff[1] = 'x';
  }
}

void parse_char(specifications *specs, char *buff, va_list *va) {
  if (specs->length != 'l') {
    char c;
    c = va_arg(*va, int);
    format_char(specs, buff, c);
  }
}

void format_char(specifications *specs, char *buff, char c) {
  if (!specs->minus && specs->width) {
    for (int i = 0; i < specs->width; i++) {
      buff[i] = ' ';
      if (i == specs->width - 1) buff[i] = c;
    }
  } else if (specs->width) {
    buff[0] = c;
    for (int i = 1; i < specs->width; i++) buff[i] = ' ';
  } else {
    buff[0] = c;
  }
}

void parse_string(specifications *specs, char *buff, va_list *va) {
  if (specs->length != 'l') {
    char *str = va_arg(*va, char *);
    format_string(specs, buff, str);
  }
}

void format_string(specifications *specs, char *buff, char *str) {
  char tmp[BUFF_SIZE] = "";
  s21_strcpy(tmp, str);
  if (specs->is_precision_set) {
    tmp[specs->precision] = '\0';
  }

  int shift = specs->width - s21_strlen(tmp);
  int len = s21_strlen(tmp);

  if (specs->minus && shift > 0) {
    s21_strcpy(buff, tmp);
    s21_memset(buff + len, ' ', shift);
  } else if (shift > 0) {
    s21_memset(buff, ' ', shift);
    s21_strcpy(buff + shift, tmp);
  } else {
    s21_strcpy(buff, tmp);
  }
}

void parse_pointer(specifications *specs, char *buff, va_list *va) {
  s21_uitoa(va_arg(*va, uint64_t), buff, 16);
  format_precision(buff, specs);
  prepend_ox(buff, specs);
  format_flags(buff, specs);
}

void parse_float(specifications *specs, char *buff, va_list *va) {
  long double val = 0;
  if (specs->length == 'L') {
    val = va_arg(*va, long double);
  } else {
    val = va_arg(*va, double);
  }

  if (!specs->is_precision_set) {
    specs->precision = 6;
  }

  s21_dtostr(val, buff, specs);
  format_flags(buff, specs);
}

void parse_float_g_G(specifications *specs, char *buff, va_list *va) {
  long double val = 0;
  if (specs->length == 'L') {
    val = va_arg(*va, long double);
  } else {
    val = va_arg(*va, double);
  }

  if (!specs->is_precision_set) {
    specs->precision = 6;
  }
  if (specs->precision == 0) specs->precision = 1;
  int precision = specs->precision;
  long double m_val = val;
  int pow = 0;
  if ((int)val - val) {
    while ((int)m_val == 0) {
      pow++;
      m_val *= 10;
    }
  }
  if (pow > 4) {
    specs->precision = 0;
    s21_dtostr(m_val, buff, specs);
  } else {
    specs->precision = 10;
    s21_dtostr(val, buff, specs);
  }
  format_gG_precision(buff, precision);
  if (pow > 4) {
    prepend_mantissa(buff, pow, '-');
  }
  remove_trailing_zeroes(buff);
  format_flags(buff, specs);
}

void remove_trailing_zeroes(char *buff) {
  int len = s21_strlen(buff);
  char *dot = s21_strchr(buff, '.');
  if (dot) {
    for (int i = len - 1; buff[i] != '.'; i--) {
      if (buff[i] == '0')
        buff[i] = '\0';
      else
        break;
    }
    if (dot[1] == '\0') dot[0] = '\0';
  }
}

void format_gG_precision(char *buff, int precision) {
  int sig_digs = 0;
  s21_size_t len = s21_strlen(buff);
  int not_zero_found = 0;
  for (s21_size_t i = 0; i < s21_strlen(buff); i++) {
    if ((buff[i] == '0' && !not_zero_found) || buff[i] == '.')
      continue;
    else
      not_zero_found = 1;

    if (isdigit(buff[i]) && not_zero_found) {
      sig_digs++;
    }
    if (sig_digs == precision && i + 1 < len) {
      int next = buff[i + 1] == '.' ? 2 : 1;
      buff[i] = buff[i + next] - '0' > 5 ? (char)(buff[i] + 1) : buff[i];
      buff[i + 1] = '\0';
      break;
    }
  }
}