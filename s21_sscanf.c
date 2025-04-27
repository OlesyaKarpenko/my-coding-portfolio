#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_string.h"
#include "s21_utils.h"

struct opt {
  int precission;
  long width;
  char length;
  char specifier;
};

int scan_char(char **str, struct opt options, char *dst);

int get_base(char specifier);
int scan_hint(char **str, struct opt options, short int *dst);
int scan_uhint(char **str, struct opt options, unsigned short int *dst);
int scan_int(char **str, struct opt options, int *dst);
int scan_uint(char **str, struct opt options, unsigned int *dst);
int scan_lint(char **str, struct opt options, long int *dst);
int scan_ulint(char **str, struct opt options, unsigned long int *dst);

int scan_float(char **str, struct opt options, float *dst);
int scan_double(char **str, struct opt options, double *dst);
int scan_ldouble(char **str, struct opt options, long double *dst);

int scan_ptr(char **str, struct opt options, void **dst);

int s21_sscanf(const char *restrict s, const char *restrict format, ...) {
  va_list ptrs;
  va_start(ptrs, format);
  char *f_ptr = (char *)format;
  char *s_ptr = (char *)s;

  int result = 0;

  while ((*f_ptr != '\0') && (*s_ptr != '\0')) {
    if (*f_ptr != '%') {
      f_ptr++;
    } else {
      struct opt options;
      options.precission = 1;
      options.width = 0;
      options.length = '\0';
      options.specifier = '\0';
      f_ptr++;
      if (*f_ptr == '*') {
        options.precission = 0;
        f_ptr++;
      }
      options.width = (s21_size_t)s21_strtol(f_ptr, &f_ptr, 10);
      if (s21_strchr("hlL", *f_ptr) != 0) {
        options.length = *f_ptr;
        f_ptr++;
      }
      options.specifier = *f_ptr;
      switch (options.specifier) {
        case 'i':
        case 'd':
          if (options.precission == 0) {
            if (options.length == 'h') {
              result += scan_hint(&s_ptr, options, s21_NULL);
            } else if (options.length == 'l') {
              result += scan_lint(&s_ptr, options, s21_NULL);
            } else if (options.length == '\0') {
              result += scan_int(&s_ptr, options, s21_NULL);
            }
          } else {
            if (options.length == 'h') {
              result += scan_hint(&s_ptr, options, va_arg(ptrs, short int *));
            } else if (options.length == 'l') {
              result += scan_lint(&s_ptr, options, va_arg(ptrs, long int *));
            } else if (options.length == '\0') {
              result += scan_int(&s_ptr, options, va_arg(ptrs, int *));
            }
          }
          break;

        case 'u':
        case 'o':
        case 'x':
        case 'X':
          if (options.precission == 0) {
            if (options.length == 'h') {
              result += scan_uhint(&s_ptr, options, s21_NULL);
            } else if (options.length == 'l') {
              result += scan_ulint(&s_ptr, options, s21_NULL);
            } else if (options.length == '\0') {
              result += scan_uint(&s_ptr, options, s21_NULL);
            }
          } else {
            if (options.length == 'l') {
              result += scan_ulint(&s_ptr, options,
                                   va_arg(ptrs, unsigned long int *));
            } else if (options.length == 'h') {
              result += scan_uhint(&s_ptr, options,
                                   va_arg(ptrs, unsigned short int *));
            } else if (options.length == '\0') {
              result +=
                  scan_uint(&s_ptr, options, va_arg(ptrs, unsigned int *));
            }
          }
          break;

        case 'c':
        case 's':
          if (options.precission == 0) {
            result += scan_char(&s_ptr, options, s21_NULL);
          } else {
            result += scan_char(&s_ptr, options, va_arg(ptrs, char *));
          }
          break;

        case 'f':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
          if (options.precission == 0) {
            if (options.length == '\0') {
              result += scan_float(&s_ptr, options, s21_NULL);
            } else if (options.length == 'l') {
              result += scan_double(&s_ptr, options, s21_NULL);
            } else if (options.length == 'L') {
              result += scan_ldouble(&s_ptr, options, s21_NULL);
            }
          } else {
            if (options.length == '\0') {
              result += scan_float(&s_ptr, options, va_arg(ptrs, float *));
            } else if (options.length == 'l') {
              result += scan_double(&s_ptr, options, va_arg(ptrs, double *));
            } else if (options.length == 'L') {
              result +=
                  scan_ldouble(&s_ptr, options, va_arg(ptrs, long double *));
            }
          }
          break;

        case 'p':
          if (options.precission == 0) {
            result += scan_ptr(&s_ptr, options, s21_NULL);
          } else {
            result += scan_ptr(&s_ptr, options, va_arg(ptrs, void **));
          }
          break;

        case 'n':
          if (options.precission == 1) {
            int *n_ptr = va_arg(ptrs, int *);
            *n_ptr = s_ptr - s;
          }
          break;
      }
    }
  }
  va_end(ptrs);
  return result;
}

int scan_char(char **str, struct opt options, char *dst) {
  if (options.specifier == 'c') {
    if (options.width == 0) {
      options.width = 1;
    }
    if (dst != s21_NULL) {
      for (int i = 0; i < options.width; i++) {
        if (**str == '\0') {
          break;
        } else {
          dst[i] = *str[i];
        }
      }
    }
    *str += options.width;
  } else if (options.specifier == 's') {
    while ((**str == ' ') || (**str == '\n')) {
      *str += 1;
    }
    char *width = *str;
    while ((*width != ' ') && (*width != '\n') && (*width != '\0')) {
      width++;
    }

    if ((options.width == 0) || (options.width > (width - *str))) {
      options.width = width - *str;
    }
    if (dst != s21_NULL) {
      s21_strncpy(dst, *str, options.width);
      dst[options.width] = '\0';
    }
    *str += options.width;
  }
  return 1;
}

int scan_hint(char **str, struct opt options, short int *dst) {
  int base = get_base(options.specifier);
  short result = 0;
  if (options.width != 0) {
    char *tmp_ptr = calloc(options.width + 1, sizeof(char));
    if (NULL != tmp_ptr) {
      s21_strncpy(tmp_ptr, *str, options.width);
      result = (short)s21_strtol(tmp_ptr, s21_NULL, base);
      free(tmp_ptr);
      tmp_ptr = s21_NULL;
      *str += options.width;
    }
  } else if (options.width == 0) {
    result = (short)s21_strtol(*str, str, base);
  }
  if (dst != s21_NULL) {
    *dst = result;
  }
  return 1;
}

int scan_uhint(char **str, struct opt options, unsigned short int *dst) {
  int base = get_base(options.specifier);
  unsigned short result = 0;
  if (options.width != 0) {
    char *tmp_ptr = calloc(options.width + 1, sizeof(char));
    if (NULL != tmp_ptr) {
      s21_strncpy(tmp_ptr, *str, options.width);
      result = (unsigned short)s21_strtol(tmp_ptr, s21_NULL, base);
      free(tmp_ptr);
      tmp_ptr = s21_NULL;
      *str += options.width;
    }
  } else if (options.width == 0) {
    result = (unsigned short)s21_strtol(*str, str, base);
  }
  if (dst != s21_NULL) {
    *dst = result;
  }
  return 1;
}

int scan_int(char **str, struct opt options, int *dst) {
  int base = get_base(options.specifier);
  int result = 0;
  if (options.width != 0) {
    char *tmp_ptr = calloc(options.width + 1, sizeof(char));
    if (NULL != tmp_ptr) {
      s21_strncpy(tmp_ptr, *str, options.width);
      result = (int)s21_strtol(tmp_ptr, s21_NULL, base);
      free(tmp_ptr);
      tmp_ptr = s21_NULL;
      *str += options.width;
    }
  } else if (options.width == 0) {
    result = (int)s21_strtol(*str, str, base);
  }
  if (dst != s21_NULL) {
    *dst = result;
  }
  return 1;
}

int scan_uint(char **str, struct opt options, unsigned int *dst) {
  int base = get_base(options.specifier);
  unsigned int result = 0;
  if (options.width != 0) {
    char *tmp_ptr = calloc(options.width + 1, sizeof(char));
    if (NULL != tmp_ptr) {
      s21_strncpy(tmp_ptr, *str, options.width);
      result = (unsigned int)s21_strtol(tmp_ptr, s21_NULL, base);
      free(tmp_ptr);
      tmp_ptr = s21_NULL;
      *str += options.width;
    }
  } else if (options.width == 0) {
    result = (unsigned int)s21_strtol(*str, str, base);
  }
  if (dst != s21_NULL) {
    *dst = result;
  }
  return 1;
}

int scan_lint(char **str, struct opt options, long int *dst) {
  int base = get_base(options.specifier);
  long result = 0;
  if (options.width != 0) {
    char *tmp_ptr = calloc(options.width + 1, sizeof(char));
    if (NULL != tmp_ptr) {
      s21_strncpy(tmp_ptr, *str, options.width);
      result = s21_strtol(tmp_ptr, s21_NULL, base);
      free(tmp_ptr);
      tmp_ptr = s21_NULL;
      *str += options.width;
    }
  } else if (options.width == 0) {
    result = s21_strtol(*str, str, base);
  }
  if (dst != s21_NULL) {
    *dst = result;
  }
  return 1;
}

int scan_ulint(char **str, struct opt options, unsigned long int *dst) {
  int base = get_base(options.specifier);
  unsigned long result = 0;
  if (options.width != 0) {
    char *tmp_ptr = calloc(options.width + 1, sizeof(char));
    if (NULL != tmp_ptr) {
      s21_strncpy(tmp_ptr, *str, options.width);
      result = (unsigned long)s21_strtol(tmp_ptr, s21_NULL, base);
      free(tmp_ptr);
      tmp_ptr = s21_NULL;
      *str += options.width;
    }
  } else if (options.width == 0) {
    result = (unsigned long)s21_strtol(*str, str, base);
  }
  if (dst != s21_NULL) {
    *dst = result;
  }
  return 1;
}

int scan_float(char **str, struct opt options, float *dst) {
  float result = 0;
  if (options.width != 0) {
    char *tmp_ptr = calloc(options.width + 1, sizeof(char));
    if (NULL != tmp_ptr) {
      s21_strncpy(tmp_ptr, *str, options.width);
      result = (float)s21_strtod(tmp_ptr, s21_NULL);
      free(tmp_ptr);
      tmp_ptr = s21_NULL;
      *str += options.width;
    }
  } else if (options.width == 0) {
    result = (float)s21_strtod(*str, str);
  }
  if (dst != s21_NULL) {
    *dst = result;
  }
  return 1;
}

int scan_double(char **str, struct opt options, double *dst) {
  double result = 0;
  if (options.width != 0) {
    char *tmp_ptr = calloc(options.width + 1, sizeof(char));
    if (NULL != tmp_ptr) {
      s21_strncpy(tmp_ptr, *str, options.width);
      result = (double)s21_strtod(tmp_ptr, s21_NULL);
      free(tmp_ptr);
      tmp_ptr = s21_NULL;
      *str += options.width;
    }
  } else if (options.width == 0) {
    result = (double)s21_strtod(*str, str);
  }
  if (dst != s21_NULL) {
    *dst = result;
  }
  return 1;
}

int scan_ldouble(char **str, struct opt options, long double *dst) {
  long double result = 0;
  if (options.width != 0) {
    char *tmp_ptr = calloc(options.width + 1, sizeof(char));
    if (NULL != tmp_ptr) {
      s21_strncpy(tmp_ptr, *str, options.width);
      result = s21_strtod(tmp_ptr, s21_NULL);
      free(tmp_ptr);
      tmp_ptr = s21_NULL;
      *str += options.width;
    }
  } else if (options.width == 0) {
    result = s21_strtod(*str, str);
  }
  if (dst != s21_NULL) {
    *dst = result;
  }
  return 1;
}

int scan_ptr(char **str, struct opt options, void **dst) {
  int base = 16;
  void *result = s21_NULL;
  if (options.width != 0) {
    char *tmp_ptr = calloc(options.width + 1, sizeof(char));
    if (NULL != tmp_ptr) {
      s21_strncpy(tmp_ptr, *str, options.width);
      result = (void *)s21_strtol(tmp_ptr, s21_NULL, base);
      free(tmp_ptr);
      tmp_ptr = s21_NULL;
      *str += options.width;
    }
  } else if (options.width == 0) {
    result = (void *)s21_strtol(*str, str, base);
  }
  if (dst != s21_NULL) {
    *dst = result;
  }
  return 1;
}

int get_base(char specifier) {
  int base = 0;
  switch (specifier) {
    case 'd':
      base = 10;
      break;

    case 'o':
      base = 8;
      break;

    case 'x':
    case 'X':
      base = 16;
      break;

    case 'i':
      base = 0;
      break;

    default:
      break;
  }
  return base;
}
