#include <errno.h>
#include <stdlib.h>

#include "s21_string.h"

char *s21_strerror(int errnum) {
  static char stringErr[200] = "";
  char *arrErre[200] = arrErr;

  if (errnum < ERROUTSIZE && errnum >= 0) {
    s21_strcpy(stringErr, arrErre[errnum]);
  } else {
    s21_sprintf(stringErr, "%s%d", ERROUT, errnum);
  }
  return stringErr;
}