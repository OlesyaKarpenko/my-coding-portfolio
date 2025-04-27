#ifndef C5_S21_DECIMAL_SRC_EXTRA_H_
#define C5_S21_DECIMAL_SRC_EXTRA_H_

#include "s21_decimal.h"

void setSign(s21_decimal *decimal, unsigned int sign);
void setSignBigDecimal(s21_big_decimal *decimal, unsigned int sign);
int getExponentFloat(float fnum);
int initDecimal(s21_decimal *decimal);
int initBigDecimal(s21_big_decimal *decimal);
int from_decimal_to_big_decimal(s21_decimal src, s21_big_decimal *dst);
int from_big_decimal_to_decimal(s21_big_decimal src, s21_decimal *dst);
int normalizeFloat(double *fnum);
unsigned int countSignificantDigits(float fnum);
void scalingFloat(double *fnum, int *scale,
                  unsigned int num_significant_digits);
int checkFirstDigitInFract(float fract);
void setScale(s21_decimal *decimal, unsigned int scale_decimal);
void setScaleBigDecimal(s21_big_decimal *decimal, unsigned int scale_decimal);
void setBit(s21_decimal *dst, unsigned int bit_value,
            unsigned int bit_position);
void setBitBigDecimal(s21_big_decimal *decimal, unsigned int bit_value,
                      unsigned int bit_position);
int writeFloatToDecimal(double src, s21_decimal *decimal, int scale_decimal);
void mulDecimalBy10(s21_decimal *decimal);
void mulBigDecimal(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result);
void mulBigDecimalBy10(s21_big_decimal *decimal);
void divBigDecimalBy10(s21_big_decimal *decimal, s21_big_decimal *remainder);
s21_big_decimal leftShiftBigDecimal(s21_big_decimal decimal, int shift);
s21_big_decimal rightShiftBigDecimal(s21_big_decimal decimal, int shift);
int getFirstNonZeroBitNumber(s21_decimal decimal);
int getFirstNonZeroBitNumberBigDecimal(s21_big_decimal decimal);
int getSign(s21_decimal decimal);
int getSignBigDecimal(s21_big_decimal decimal);
unsigned int getScale(s21_decimal decimal);
unsigned int getScaleBigDecimal(s21_big_decimal decimal);
unsigned int checkDecimalIsZero(s21_decimal decimal);
unsigned int getBit(s21_decimal decimal, unsigned int bit_position);
unsigned int getBitBigDecimal(s21_big_decimal decimal,
                              unsigned int bit_position);
void getTwosComplementBigDecimal(s21_big_decimal src, s21_big_decimal *dst);
void inverseBigDecimal(s21_big_decimal *src);
void addValuesBigDecimal(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal *result);
void subValuesBigDecimal(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal *result);
void copyBigDecimalToDecimal(s21_big_decimal src, s21_decimal *dst);
int divBigDecimal(s21_big_decimal value_1, s21_big_decimal value_2,
                  s21_big_decimal *result);
void divWithRemainder(s21_big_decimal value_1, s21_big_decimal value_2,
                      s21_big_decimal *unit, s21_big_decimal *remainder);
void bankRounding(s21_big_decimal *decimal, s21_big_decimal remainder);
void correctionDecimal(s21_decimal *decimal);

// void printDecimal(s21_decimal decimal);
// void printBigDecimal(s21_big_decimal decimal);
// void printFloatInBites(float fnum);
// void printDecimalInBits(s21_decimal decimal);
// void printBigDecimalInBits(s21_big_decimal decimal);

#endif  // C5_S21_DECIMAL_SRC_EXTRA_H_
