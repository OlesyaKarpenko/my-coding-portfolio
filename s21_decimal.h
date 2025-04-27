#ifndef C5_S21_DECIMAL_SRC_S21_DECIMAL_H_
#define C5_S21_DECIMAL_SRC_S21_DECIMAL_H_

#include <math.h>
#include <string.h>

#define DEFAULT_PRECISION 7
#define MAX_BIT_POSITION 95
#define MAX_BIT_POSITION_BIG_DECIMAL 223
#define MAX_NUM_OF_BITS_IN_A_WORD 32
#define MAX_EXPONENT_OF_FLOAT 95
#define MIN_EXPONENT_OF_FLOAT -95
#define MAX_NUM_OF_BYTE_DECIMAL 4
#define MAX_NUM_OF_BYTES_BIG_DECIMAL 8
#define SCALE_BYTE_BIG_DECIMAL 7
#define MAX_NUM_SCALE_DECIMAL 28

typedef enum CodeErrors { OK, INF, N_INF, DIV_BY_ZERO } CodeErrors;

typedef enum ConvertationErrors {
  OK_CONVERTATION,
  ERROR_CONVERTATION
} ConvertationErrors;

typedef enum WordsDecimal { LOW, MID, HIGH, SCALE } WordsDecimal;

typedef enum SignDecimal { POSITIVE, NEGATIVE } SignDecimal;

typedef enum BoolType { FALSE, TRUE } BoolType;

typedef struct s21_decimal {
  unsigned int bits[4];
} s21_decimal;

typedef struct s21_big_decimal {
  unsigned int bits[8];
} s21_big_decimal;

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

#endif  // C5_S21_DECIMAL_SRC_S21_DECIMAL_H_
