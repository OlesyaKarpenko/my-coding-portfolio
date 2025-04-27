#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  s21_size_t i;
  for (i = 0; i < n; ++i) {
    if (*((char *)str + i) == c) {
      return (char *)str + i;
    }
  }
  return s21_NULL;
}