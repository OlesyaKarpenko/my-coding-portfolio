#include <stdlib.h>

#include "s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *buff = s21_NULL;
  if (str != s21_NULL && src != s21_NULL) {
    s21_size_t n_str = s21_strlen(str);
    s21_size_t n_src = s21_strlen(src);
    s21_size_t sum_line = n_str + n_src;
    if (n_src >= start_index) {
      buff = (char *)malloc(sizeof(char) * (sum_line + 1));
    }
    if (buff) {
      s21_size_t i = 0, j = 0, k = 0;
      while (i < sum_line) {
        if (i >= start_index && j < n_str) {
          buff[i] = str[j];
          j++;
        } else {
          buff[i] = src[k];
          k++;
        }
        i++;
      }
      buff[sum_line] = '\0';
    }
  }
  return buff;
}