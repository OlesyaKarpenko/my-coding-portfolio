#include <stdlib.h>

#include "s21_string.h"
#include "s21_utils.h"

void *s21_trim(const char *src, const char *trim_chars) {
  char *result = s21_NULL;

  if (src) {
    if (trim_chars && *trim_chars) {
      s21_size_t len_src = s21_strlen(src);
      s21_size_t len_chars = s21_strlen(trim_chars);

      s21_trim_l(&src, trim_chars, &len_src, len_chars);
      if (len_src) s21_trim_r(&src, trim_chars, &len_src, len_chars);

      result = (char *)malloc(sizeof(char) * (len_src + 1));

      if (result) {
        for (s21_size_t i = 0; i < len_src + 1; i++) {
          if (i < len_src) {
            result[i] = src[i];
          } else {
            result[i] = '\0';
          }
        }
      }
    } else {
      result = s21_trim(src, " \t\n");
    }
  }

  return result;
}

void s21_trim_l(const char **src, const char *trim_chars, s21_size_t *src_len,
                const s21_size_t trim_chars_len) {
  s21_size_t m = 0;

  while (src && m != trim_chars_len) {
    if ((**src) == trim_chars[m]) {
      (*src)++;
      (*src_len) -= 1;
      m = 0;
      continue;
    }
    m++;
  }
}

void s21_trim_r(const char **src, const char *trim_chars, s21_size_t *src_len,
                const s21_size_t trim_chars_len) {
  s21_size_t m = 0;
  s21_size_t i = (*src_len) - 1;

  while (src && m != trim_chars_len) {
    if ((*src)[i] == trim_chars[m]) {
      i--;
      (*src_len)--;
      m = 0;
      continue;
    }

    m++;
  }
}