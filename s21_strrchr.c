

#include "s21_string.h"

char *s21_strrchr(const char *str, int c) {
  char *save_c;
  char ch;

  for (save_c = (char *)0; (ch = *str); str++) {
    if (ch == c) save_c = (char *)str;
  }
  return save_c;
}