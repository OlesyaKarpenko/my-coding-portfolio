#include <stdlib.h>

#include "s21_string.h"

void *s21_to_upper(const char *str) {
  char *buff = s21_NULL;
  if (str != s21_NULL) {
    s21_size_t n = s21_strlen(str);

    buff = (char *)malloc(sizeof(char) * (n + 1));
#ifdef MALLOC_FAIL_VALUE
    if (n == MALLOC_FAIL_VALUE) {
      free(buff);
      buff = s21_NULL;
    }
#endif

    if (buff) {
      for (s21_size_t i = 0; i <= n; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
          buff[i] = (str[i] - 'a') + 'A';
        } else {
          buff[i] = str[i];
        }
      }
      buff[n] = '\0';
    }
  }

  return buff;
}