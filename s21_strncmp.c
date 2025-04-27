#include "s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  const unsigned char *pstr1 = (unsigned char *)str1,
                      *pstr2 = (unsigned char *)str2;
  int result = 0, fend = 0;
  while ((n != 0) && fend == 0) {
    if (*pstr1 != *pstr2) {
      result = (unsigned char)*pstr1 - (unsigned char)*pstr2;
      fend = 1;
    } else {
      pstr1++;
      pstr2++;
    }
    n--;
  }
  return result;
}