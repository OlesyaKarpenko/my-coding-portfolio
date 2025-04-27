#include "s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  if (n != 0) {
    char *dest0 = (char *)dest;
    char *src0 = (char *)src;

    while (n--) {
      *dest0++ = *src0++;
    }
  }

  return dest;
}
