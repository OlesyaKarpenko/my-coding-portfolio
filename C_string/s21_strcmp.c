#include "s21_string.h"

int s21_strcmp(const char *str1, const char *str2) {
  int result = 0, fend = 0;
  while (fend == 0) {
    if (*str1 == *str2) {
      str1++;
      str2++;
      if (*str1 == '\0') {
        result = 0;
        fend = 1;
      }
    } else {
      result = ((*str1 < *str2) ? -1 : 1);
      fend = 1;
    }
  }
  return result;
}
