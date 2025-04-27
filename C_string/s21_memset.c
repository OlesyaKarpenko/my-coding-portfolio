#include "s21_string.h"

void *s21_memset(void *str, int c, s21_size_t n) {
  char *str0 = (char *)str;

  while (n--) {
    *str0++ = c;
  }

  return str;
}
