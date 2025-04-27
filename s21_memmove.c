#include "s21_string.h"
#include "stdlib.h"

void *s21_memmove(void *dest, const void *src, s21_size_t n) {
  if (n != 0) {
    char *dest0 = (char *)dest;
    char *src0 = (char *)src;

    char *tmp = (char *)malloc(sizeof(char) * n);
#ifdef MALLOC_FAIL_VALUE
    if (n == MALLOC_FAIL_VALUE) {
      free(tmp);
      tmp = s21_NULL;
    }
#endif

    if (tmp != s21_NULL) {
      s21_memcpy(tmp, src0, n);
      s21_memcpy(dest0, tmp, n);
      free(tmp);
    }
  }

  return dest;
}
