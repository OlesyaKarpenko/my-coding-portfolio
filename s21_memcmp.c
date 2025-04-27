#include "s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int result = 0, fend = 0;
  const unsigned char *pstr1 = (unsigned char *)str1,
                      *pstr2 = (unsigned char *)str2;
  while (n != 0 && fend == 0) {
    result = (unsigned char)*pstr1 - (unsigned char)*pstr2;
    if (*pstr1 != *pstr2) {
      fend = 1;
    }
    pstr1++;
    pstr2++;
    n--;
  }
  return result;
}