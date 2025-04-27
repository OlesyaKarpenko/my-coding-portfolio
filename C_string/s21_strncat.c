#include "s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *ptr = dest;
  while (*ptr != '\0') {
    ptr++;
  }

  s21_size_t i = 0;
  while (*src != '\0' && i < n) {
    *ptr++ = *src++;
    i++;
  }
  *ptr = '\0';
  return dest;
}
