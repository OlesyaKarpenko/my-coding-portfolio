#include "s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *p = s21_NULL;
  if (str)
    p = str;
  else
    return s21_NULL;
  str = p + s21_strspn(p, delim);
  p = str + s21_strcspn(str, delim);
  if (p == str) return p = 0;
  p = *p ? *p = 0, p + 1 : 0;
  return str;
}