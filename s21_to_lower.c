#include <stdlib.h>

#include "s21_string.h"

void *s21_to_lower(const char *str) {
  char *buff = s21_NULL;
  if (str != s21_NULL) {
    s21_size_t n = s21_strlen(str);
    buff = (char *)malloc(sizeof(char) * (n + 1));
    if (buff) {
      for (s21_size_t i = 0; i <= n; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
          buff[i] = (str[i] - 'A') + 'a';
        } else {
          buff[i] = str[i];
        }
      }
      buff[n] = '\0';
    }
  }
  return buff;
}