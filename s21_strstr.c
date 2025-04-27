#include "s21_string.h"
int comp(const char *haystack, const char *needle);

char *s21_strstr(const char *haystack, const char *needle) {
  while (*haystack != '\0') {
    if ((*haystack == *needle) && comp(haystack, needle))
      return (char *)haystack;
    haystack++;
  }
  return s21_NULL;
}

int comp(const char *haystack, const char *needle) {
  while (*haystack && *needle) {
    if (*haystack != *needle) return 0;
    haystack++;
    needle++;
  }
  return (*needle == '\0');
}