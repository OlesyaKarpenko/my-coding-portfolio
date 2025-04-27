#include "s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  int str1_len = s21_strlen(str1);
  int str2_len = s21_strlen(str2);
  for (int i = 0; i < str1_len; i++) {
    for (int j = 0; j < str2_len; j++) {
      if (str1[i] == str2[j]) return (char *)&str1[i];
    }
  }
  return s21_NULL;
}