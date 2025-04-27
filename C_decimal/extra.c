#include "extra.h"

/// @brief | Устанавливает значение bit_value указанного бита bit_position в
/// числе decimal|
/// @param dst -  указатель на число s21_decimal
/// @param bit_value -  значение бита uint
/// @param bit_position -  номер бита uint
void setBit(s21_decimal* decimal, unsigned int bit_value,
            unsigned int bit_position) {
  if (decimal != NULL) {
    unsigned int mask = 0x1;
    decimal->bits[bit_position / MAX_NUM_OF_BITS_IN_A_WORD] &=
        ~(mask << bit_position % MAX_NUM_OF_BITS_IN_A_WORD);
    decimal->bits[bit_position / MAX_NUM_OF_BITS_IN_A_WORD] |=
        bit_value << bit_position % MAX_NUM_OF_BITS_IN_A_WORD;
  }
}

/// @brief | Устанавливает значение bit_value указанного бита bit_position в
/// числе decimal|
/// @param dst -  указатель на число s21_big_decimal
/// @param bit_value -  значение бита uint
/// @param bit_position -  номер бита uint
void setBitBigDecimal(s21_big_decimal* decimal, unsigned int bit_value,
                      unsigned int bit_position) {
  if (decimal != NULL) {
    unsigned int mask = 0x1;
    decimal->bits[bit_position / MAX_NUM_OF_BITS_IN_A_WORD] &=
        ~(mask << bit_position % MAX_NUM_OF_BITS_IN_A_WORD);
    decimal->bits[bit_position / MAX_NUM_OF_BITS_IN_A_WORD] |=
        bit_value << bit_position % MAX_NUM_OF_BITS_IN_A_WORD;
  }
}

/// @brief | Возвращает значение указанного бита bit_position в числе decimal |
/// @param decimal -  число s21_decimal
/// @param bit_position -  номер бита uint
/// @return 0 или 1
unsigned int getBit(s21_decimal decimal, unsigned int bit_position) {
  unsigned int mask = 0x1;
  return !!(decimal.bits[bit_position / MAX_NUM_OF_BITS_IN_A_WORD] &
            mask << bit_position % MAX_NUM_OF_BITS_IN_A_WORD);
}

/// @brief | Возвращает значение указанного бита bit_position в числе decimal |
/// @param decimal -  число s21_big_decimal
/// @param bit_position -  номер бита uint
/// @return 0 или 1
unsigned int getBitBigDecimal(s21_big_decimal decimal,
                              unsigned int bit_position) {
  unsigned int mask = 0x1;
  return !!(decimal.bits[bit_position / MAX_NUM_OF_BITS_IN_A_WORD] &
            mask << bit_position % MAX_NUM_OF_BITS_IN_A_WORD);
}

/// @brief | Устанавливает знак числа sign в числе decimal |
/// @param decimal -  указатель на число s21_decimal
/// @param sign -  знак числа uint 0 или 1
void setSign(s21_decimal* decimal, unsigned int sign) {
  if (decimal != NULL) {
    unsigned int mask = 0x7FFFFFFF;
    decimal->bits[SCALE] &= mask;
    mask = sign << 31;
    decimal->bits[SCALE] |= mask;
  }
}

/// @brief | Устанавливает знак числа sign в числе decimal |
/// @param decimal -  указатель на число s21_big_decimal
/// @param sign -  знак числа uint 0 или 1
void setSignBigDecimal(s21_big_decimal* decimal, unsigned int sign) {
  if (decimal != NULL) {
    unsigned int mask = 0x7FFFFFFF;
    decimal->bits[SCALE_BYTE_BIG_DECIMAL] &= mask;
    mask = sign << 31;
    decimal->bits[SCALE_BYTE_BIG_DECIMAL] |= mask;
  }
}

/// @brief | Возвращает степень числа fnum |
/// @param fnum -  число float
/// @return степень числа float (int)
int getExponentFloat(float fnum) {
  int mask = 0x0;
  memcpy(&mask, &fnum, 4);
  return (mask >> 23 & 0xFF) - 127;
}

/// @brief | Инициализация числа decimal |
/// @param decimal - указатель на число s21_decimal
/// @return
/// 0 - OK;
/// 1 - ошибка инициализации
int initDecimal(s21_decimal* decimal) {
  int result = OK_CONVERTATION;
  if (decimal == NULL) {
    result = ERROR_CONVERTATION;
  } else {
    decimal->bits[LOW] = decimal->bits[MID] = decimal->bits[HIGH] =
        decimal->bits[SCALE] = 0;
  }
  return result;
}

/// @brief | Инициализация числа big_decimal |
/// @param big_decimal - указатель на число s21_big_decimal
/// @return
/// 0 - OK;
/// 1 - ошибка инициализации
int initBigDecimal(s21_big_decimal* decimal) {
  int result = OK_CONVERTATION;
  if (decimal == NULL) {
    result = ERROR_CONVERTATION;
  } else {
    for (unsigned int num_byte = 0; num_byte < MAX_NUM_OF_BYTES_BIG_DECIMAL;
         num_byte++) {
      decimal->bits[num_byte] = 0;
    }
  }
  return result;
}

/// @brief | Нормализация числа fnum |
/// @param fnum - указатель на число double
/// @return scale для записи в s21_decimal (int)
int normalizeFloat(double* fnum) {
  int scale = 0;
  unsigned int amount_significant_digits = 0;
  if (fnum != NULL) {
    while (amount_significant_digits < DEFAULT_PRECISION &&
           scale < MAX_NUM_SCALE_DECIMAL) {
      *fnum *= 10.0;
      amount_significant_digits = countSignificantDigits(*fnum);
      scale++;
    }
  }
  return scale;
}

/// @brief | Подсчитывает количество цифр в целой части числа fnum |
/// @param fnum - число float
/// @return количество цифр в целой части числа float (uint)
unsigned int countSignificantDigits(float fnum) {
  unsigned int amount_significant_digits = 0;
  while (fnum >= 1.0) {
    fnum /= 10.0;
    amount_significant_digits++;
  }
  return amount_significant_digits;
}

/// @brief | Добавляет указанное количество значащих цифр
/// amount_significant_digits к целой части числа fnum |
/// @param fnum - указатель на число double
/// @param  scale - указатель на scale (int) числа s21_decimal
/// @param  amount_significant_digits - количество значащих цифр (int)
void scalingFloat(double* fnum, int* scale,
                  unsigned int amount_significant_digits) {
  if (fnum != NULL && scale != NULL) {
    double fract = 0.0;
    if (amount_significant_digits < DEFAULT_PRECISION &&
        *scale < MAX_NUM_SCALE_DECIMAL) {
      for (unsigned int i = 0;
           i < DEFAULT_PRECISION - amount_significant_digits; i++, (*scale)++) {
        *fnum *= 10.0;
      }
    } else if (amount_significant_digits > DEFAULT_PRECISION) {
      for (unsigned int i = 0;
           i < amount_significant_digits - DEFAULT_PRECISION; i++, (*scale)--) {
        *fnum /= 10.0;
      }
    }
    fract = modf(*fnum, fnum);
    *fnum += checkFirstDigitInFract(fract);
  }
}

/// @brief | Проверяет первую цифру в дробной части числа fract |
/// @param fract - дробная часть числа float
/// @return
/// 0 - если цифра < 5;
/// 1 - если цифра >= 5
int checkFirstDigitInFract(float fract) { return (int)(fract * 10.0) >= 5; }

/// @brief | Записывает значение scale_decimal в число decimal |
/// @param decimal - указатель на число s21_decimal
/// @param scale_decimal - scale (uint) числа s21_decimal
void setScale(s21_decimal* decimal, unsigned int scale_decimal) {
  if (decimal != NULL) {
    unsigned int mask = 0x1FF;
    decimal->bits[SCALE] &= ~(mask << 16);
    decimal->bits[SCALE] |= (scale_decimal << 16);
  }
}

/// @brief | Записывает значение scale_decimal в число decimal |
/// @param decimal - указатель на число s21_big_decimal
/// @param scale_decimal - scale (uint) числа s21_big_decimal
void setScaleBigDecimal(s21_big_decimal* decimal, unsigned int scale_decimal) {
  if (decimal != NULL) {
    unsigned int mask = 0x1FF;
    decimal->bits[SCALE_BYTE_BIG_DECIMAL] &= ~(mask << 16);
    decimal->bits[SCALE_BYTE_BIG_DECIMAL] |= (scale_decimal << 16);
  }
}

/// @brief | Записывает подготовленное число src в число decimal |
/// @param src - число double
/// @param decimal - указатель на число s21_decimal
/// @param scale_decimal - scale (int) числа s21_decimal
/// @return
/// 0 - OK;
/// 1 - ошибка записи
int writeFloatToDecimal(double src, s21_decimal* decimal, int scale_decimal) {
  int result = OK_CONVERTATION;
  if (decimal == NULL) {
    result = ERROR_CONVERTATION;
  } else {
    unsigned int bit_position = 0;
    while (src >= 1.0 && bit_position <= MAX_BIT_POSITION) {
      setBit(decimal, floorf(fmodf(src, 2.0)), bit_position);
      src /= 2.0;
      bit_position++;
    }
    if (src > 1.0) {
      initDecimal(decimal);
      result = ERROR_CONVERTATION;
    } else {
      while (scale_decimal < 0) {
        mulDecimalBy10(decimal);
        scale_decimal++;
      }
      setScale(decimal, scale_decimal);
      correctionDecimal(decimal);
    }
  }
  return result;
}

/// @brief | Убирает незначащие нули в числе decimal |
/// @param decimal - указатель число s21_decimal
void correctionDecimal(s21_decimal* decimal) {
  if (decimal != NULL) {
    unsigned int scale = getScale(*decimal);
    if (scale) {
      s21_big_decimal tmp = {0};
      s21_big_decimal remainder = {0};
      int non_zero_bit = 0;
      from_decimal_to_big_decimal(*decimal, &tmp);
      s21_big_decimal result = tmp;
      do {
        divBigDecimalBy10(&tmp, &remainder);
        scale--;
        non_zero_bit = getFirstNonZeroBitNumberBigDecimal(remainder);
        if (non_zero_bit == -1) {
          result = tmp;
        }
      } while (scale && non_zero_bit == -1);
      from_big_decimal_to_decimal(result, decimal);
    }
  }
}

/// @brief | Преобразование s21_decimal в s21_big_decimal |
/// @param src - число s21_decimal
/// @param dst - указатель на число s21_big_decimal
/// @return
/// 0 - OK;
/// 1 - ошибка конвертации
int from_decimal_to_big_decimal(s21_decimal src, s21_big_decimal* dst) {
  int result = OK;
  if (dst == NULL) {
    result = ERROR_CONVERTATION;
  } else {
    initBigDecimal(dst);
    for (unsigned int num_byte = 0; num_byte < 3; num_byte++) {
      dst->bits[num_byte] = src.bits[num_byte];
    }
    dst->bits[SCALE_BYTE_BIG_DECIMAL] = src.bits[SCALE];
  }
  return result;
}

/// @brief | Преобразование s21_big_decimal в s21_decimal |
/// @param src - число s21_big_decimal
/// @param dst - указатель на число s21_decimal
/// @return
/// 0 - OK;
/// 1 - число слишком большое (INF);
/// 2 - число слишком маленькое (N_INF);
/// 3 - NAN
int from_big_decimal_to_decimal(s21_big_decimal src, s21_decimal* dst) {
  int result = OK;
  if (dst == NULL) {
    result = DIV_BY_ZERO;
  } else {
    initDecimal(dst);
    int non_zero_num_bit = getFirstNonZeroBitNumberBigDecimal(src);
    unsigned int scale = getScaleBigDecimal(src);
    if (non_zero_num_bit == -1 || (non_zero_num_bit <= MAX_BIT_POSITION &&
                                   scale <= MAX_NUM_SCALE_DECIMAL)) {
      copyBigDecimalToDecimal(src, dst);
    } else {
      s21_big_decimal remainder = {0};
      s21_big_decimal tmp = {0};
      unsigned int sign = getSignBigDecimal(src);
      setSignBigDecimal(&src, POSITIVE);
      while (!(scale == 0 || (non_zero_num_bit <= MAX_BIT_POSITION &&
                              scale <= MAX_NUM_SCALE_DECIMAL))) {
        divBigDecimalBy10(&src, &tmp);
        addValuesBigDecimal(remainder, tmp, &remainder);
        non_zero_num_bit = getFirstNonZeroBitNumberBigDecimal(src);
        scale = getScaleBigDecimal(src);
      }
      if (non_zero_num_bit <= MAX_BIT_POSITION &&
          scale <= MAX_NUM_SCALE_DECIMAL) {
        bankRounding(&src, remainder);
        non_zero_num_bit = getFirstNonZeroBitNumberBigDecimal(src);
        if (non_zero_num_bit > MAX_BIT_POSITION && scale != 0) {
          divBigDecimalBy10(&src, &remainder);
          bankRounding(&src, remainder);
          setSignBigDecimal(&src, sign);
          copyBigDecimalToDecimal(src, dst);
        } else if (non_zero_num_bit <= MAX_BIT_POSITION) {
          setSignBigDecimal(&src, sign);
          copyBigDecimalToDecimal(src, dst);
        } else if (!sign) {
          result = INF;
        } else if (sign) {
          result = N_INF;
        }
      } else {
        if (scale > MAX_NUM_SCALE_DECIMAL) {
          result = N_INF;
        } else if (!sign) {
          result = INF;
        } else {
          result = N_INF;
        }
      }
    }
  }
  return result;
}

/// @brief | Банковское округление числа s21_big_decimal |
/// @param decimal - указатель на число s21_big_decimal
/// @param remainder - число s21_big_decimal
void bankRounding(s21_big_decimal* decimal, s21_big_decimal remainder) {
  if (decimal != NULL) {
    unsigned int sign = getSignBigDecimal(*decimal);
    setSignBigDecimal(decimal, POSITIVE);
    setSignBigDecimal(&remainder, POSITIVE);
    s21_big_decimal half_divisor = {{5, 0, 0, 0, 0, 0, 0, 0}};
    unsigned int scale_decimal = getScaleBigDecimal(*decimal);
    unsigned int scale_divisor = scale_decimal + 1;
    setScaleBigDecimal(&half_divisor, scale_divisor);
    unsigned int scale_remainder = getScaleBigDecimal(remainder);
    while (scale_divisor < scale_remainder) {
      mulBigDecimalBy10(&half_divisor);
      scale_divisor++;
    }
    s21_big_decimal unit = {{1, 0, 0, 0, 0, 0, 0, 0}};
    setScaleBigDecimal(&unit, scale_decimal);
    int bit_position_remainder = getFirstNonZeroBitNumberBigDecimal(remainder);
    int bit_position_divisor = getFirstNonZeroBitNumberBigDecimal(half_divisor);
    if (bit_position_remainder > bit_position_divisor) {
      addValuesBigDecimal(*decimal, unit, decimal);
    } else if (bit_position_remainder == bit_position_divisor) {
      unsigned int bit_value_remainder = 0;
      unsigned int bit_value_divisor = 0;
      int num_bit = 0;
      for (num_bit = bit_position_remainder;
           bit_value_remainder == bit_value_divisor && num_bit >= 0;
           num_bit--) {
        bit_value_remainder = getBitBigDecimal(remainder, num_bit);
        bit_value_divisor = getBitBigDecimal(half_divisor, num_bit);
      }
      if (num_bit == -1) {
        unsigned int bit_value = getBitBigDecimal(*decimal, 0);
        if (bit_value) {
          addValuesBigDecimal(*decimal, unit, decimal);
        }
      } else if (bit_value_remainder) {
        addValuesBigDecimal(*decimal, unit, decimal);
      }
    }
    setSignBigDecimal(decimal, sign);
  }
}

/// @brief | Копирует побайтово src в dst |
/// @param src -  число s21_big_decimal
/// @param dst -  указатель на число s21_decimal
void copyBigDecimalToDecimal(s21_big_decimal src, s21_decimal* dst) {
  if (dst != NULL) {
    for (unsigned int num_byte = 0; num_byte < MAX_NUM_OF_BYTE_DECIMAL - 1;
         num_byte++) {
      dst->bits[num_byte] = src.bits[num_byte];
    }
    dst->bits[SCALE] = src.bits[SCALE_BYTE_BIG_DECIMAL];
  }
}

/// @brief | Умножение двух чисел s21_big_decimal |
/// @param value_1 -  число s21_big_decimal
/// @param value_2 -  число s21_big_decimal
/// @param result -  указатель на s21_big_decimal
void mulBigDecimal(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal* result) {
  if (result != NULL) {
    s21_big_decimal tmp = {0};
    initBigDecimal(result);
    unsigned int bit_value_2 = 0;
    for (int bit_position = 0;
         bit_position <= getFirstNonZeroBitNumberBigDecimal(value_2);
         bit_position++) {
      bit_value_2 = getBitBigDecimal(value_2, bit_position);
      if (bit_value_2) {
        tmp = leftShiftBigDecimal(value_1, bit_position);
        setSignBigDecimal(&tmp, POSITIVE);
        setScaleBigDecimal(&tmp, 0);
        addValuesBigDecimal(tmp, *result, result);
      }
    }
    unsigned int scale_value_1 = getScaleBigDecimal(value_1);
    unsigned int scale_value_2 = getScaleBigDecimal(value_2);
    setScaleBigDecimal(result, scale_value_1 + scale_value_2);
    unsigned int sign_value_1 = getSignBigDecimal(value_1);
    unsigned int sign_value_2 = getSignBigDecimal(value_2);
    setSignBigDecimal(result, (sign_value_1 + sign_value_2) % 2);
  }
}

/// @brief | Умножает число decimal на 10 |
/// @param decimal - указатель на число s21_decimal
/// @return
/// 0 - OK;
/// 1 - число слишком большое (INF)
void mulDecimalBy10(s21_decimal* decimal) {
  if (decimal != NULL) {
    s21_decimal value_10 = {{10, 0, 0, 0}};
    s21_mul(*decimal, value_10, decimal);
    unsigned int scale = getScale(*decimal);
    setScale(decimal, ++scale);
  }
}

/// @brief | Умножает число decimal на 10 |
/// @param decimal - указатель на число s21_big_decimal
void mulBigDecimalBy10(s21_big_decimal* decimal) {
  if (decimal != NULL) {
    s21_big_decimal value_10 = {{10, 0, 0, 0, 0, 0, 0, 0}};
    mulBigDecimal(*decimal, value_10, decimal);
    unsigned int scale = getScaleBigDecimal(*decimal);
    setScaleBigDecimal(decimal, ++scale);
  }
}

/// @brief | Делит два числа s21_big_decimal |
/// @param value_1 - число s21_big_decimal
/// @param value_2 - число s21_big_decimal
/// @param result - указатель на число s21_big_decimal
/// @param remainder - указатель на число s21_big_decimal
int divBigDecimal(s21_big_decimal value_1, s21_big_decimal value_2,
                  s21_big_decimal* result) {
  int output = OK;
  if (result == NULL) {
    output = DIV_BY_ZERO;
  } else {
    initBigDecimal(result);
    s21_big_decimal remainder = {0};
    unsigned int sign_value_1 = getSignBigDecimal(value_1);
    unsigned int sign_value_2 = getSignBigDecimal(value_2);
    unsigned int sign_result = (sign_value_1 + sign_value_2) % 2;
    setSignBigDecimal(&value_1, POSITIVE);
    setSignBigDecimal(&value_2, POSITIVE);
    unsigned int scale_value_1 = getScaleBigDecimal(value_1);
    unsigned int scale_value_2 = getScaleBigDecimal(value_2);
    if (scale_value_1 != scale_value_2) {
      if (scale_value_1 < scale_value_2) {
        while (scale_value_1 < scale_value_2) {
          mulBigDecimalBy10(&value_1);
          scale_value_1++;
        }
      } else if (scale_value_2 < scale_value_1) {
        while (scale_value_2 < scale_value_1) {
          mulBigDecimalBy10(&value_2);
          scale_value_2++;
        }
      }
    }
    unsigned int scale_result = 0;
    int non_zero_bit_result = 0;
    int non_zero_bit_remainder = 0;
    s21_big_decimal unit = {0};
    do {
      divWithRemainder(value_1, value_2, &unit, &remainder);
      setScaleBigDecimal(&remainder, scale_value_1);
      scale_result = getScaleBigDecimal(*result);
      setScaleBigDecimal(&unit, scale_result);
      addValuesBigDecimal(*result, unit, result);
      non_zero_bit_result = getFirstNonZeroBitNumberBigDecimal(*result);
      non_zero_bit_remainder = getFirstNonZeroBitNumberBigDecimal(remainder);
      if (non_zero_bit_result <= MAX_BIT_POSITION &&
          non_zero_bit_remainder != -1) {
        mulBigDecimalBy10(result);
        mulBigDecimalBy10(&remainder);
        value_1 = remainder;
        scale_value_1 = getScaleBigDecimal(value_1);
        setScaleBigDecimal(&value_2, scale_value_1);
      }
    } while (non_zero_bit_result <= MAX_BIT_POSITION &&
             non_zero_bit_remainder != -1);
    setSignBigDecimal(result, sign_result);
  }
  return output;
}

/// @brief | Возвращает целую часть и остаток деления двух чисел s21_big_decimal
/// |
/// @param value_1 - число s21_big_decimal
/// @param value_2 - число s21_big_decimal
/// @param result - указатель на число s21_big_decimal
/// @param remainder - указатель на число s21_big_decimal
void divWithRemainder(s21_big_decimal value_1, s21_big_decimal value_2,
                      s21_big_decimal* unit, s21_big_decimal* remainder) {
  if (unit != NULL && remainder != NULL) {
    initBigDecimal(unit);
    initBigDecimal(remainder);
    unsigned int scale_value_1 = getScaleBigDecimal(value_1);
    unsigned int scale_value_2 = getScaleBigDecimal(value_2);
    if (scale_value_1 != scale_value_2) {
      if (scale_value_1 < scale_value_2) {
        while (scale_value_1 < scale_value_2) {
          mulBigDecimalBy10(&value_1);
          scale_value_1++;
        }
      } else if (scale_value_2 < scale_value_1) {
        while (scale_value_2 < scale_value_1) {
          mulBigDecimalBy10(&value_2);
          scale_value_2++;
        }
      }
    }
    unsigned int non_zero_bit_value_1 =
        getFirstNonZeroBitNumberBigDecimal(value_1);
    unsigned int non_zero_bit_value_2 =
        getFirstNonZeroBitNumberBigDecimal(value_2);
    int shift = 0;
    while (non_zero_bit_value_2 < non_zero_bit_value_1) {
      value_2 = leftShiftBigDecimal(value_2, 1);
      non_zero_bit_value_2 = getFirstNonZeroBitNumberBigDecimal(value_2);
      shift++;
    }
    s21_big_decimal tmp = {0};
    unsigned int sign_tmp = 0;
    for (int i = 0; i <= shift; i++) {
      if (i == 0) {
        subValuesBigDecimal(value_1, value_2, &tmp);
      } else {
        if (!sign_tmp) {
          subValuesBigDecimal(value_1, value_2, &tmp);
        } else {
          addValuesBigDecimal(value_1, value_2, &tmp);
        }
      }
      sign_tmp = getSignBigDecimal(tmp);
      if (!sign_tmp) {
        setBitBigDecimal(unit, 1, shift - i);
      } else {
        setBitBigDecimal(unit, 0, shift - i);
      }
      if (i != shift) {
        value_1 = leftShiftBigDecimal(tmp, 1);
      } else {
        int non_zero_bit = getFirstNonZeroBitNumberBigDecimal(tmp);
        if (sign_tmp && non_zero_bit != -1) {
          addValuesBigDecimal(tmp, value_2, &tmp);
        }
        tmp = rightShiftBigDecimal(tmp, shift);
        *remainder = tmp;
      }
    }
  }
}

/// @brief | Делит число decimal на 10 |
/// @param decimal - указатель на число s21_big_decimal
void divBigDecimalBy10(s21_big_decimal* decimal, s21_big_decimal* remainder) {
  if (decimal != NULL && remainder != NULL) {
    s21_big_decimal divident = *decimal;
    s21_big_decimal divisor = {{10, 0, 0, 0, 0, 0, 0, 0}};
    unsigned int sign_result = getSignBigDecimal(*decimal);
    unsigned scale_divident = getScaleBigDecimal(divident);
    setSignBigDecimal(&divident, POSITIVE);
    setScaleBigDecimal(&divisor, scale_divident);
    initBigDecimal(decimal);
    initBigDecimal(remainder);
    divWithRemainder(divident, divisor, decimal, remainder);
    setSignBigDecimal(decimal, sign_result);
    setSignBigDecimal(remainder, sign_result);
    setScaleBigDecimal(decimal, scale_divident - 1);
    setScaleBigDecimal(remainder, scale_divident);
  }
}

/// @brief | Побитовый левый сдвиг числа decimal на заданное число битов shift
/// |
/// @param decimal - число s21_big_decimal
/// @param shift - расстояние сдвига (int)
/// @return
/// число s21_big_decimal
s21_big_decimal leftShiftBigDecimal(s21_big_decimal decimal, int shift) {
  int bit_position = getFirstNonZeroBitNumberBigDecimal(decimal);
  s21_big_decimal result = decimal;
  unsigned int bit_value = 0;
  for (; bit_position >= 0 && shift; bit_position--) {
    bit_value = getBitBigDecimal(result, bit_position);
    setBitBigDecimal(&result, 0, bit_position);
    setBitBigDecimal(&result, bit_value, bit_position + shift);
  }
  return result;
}

/// @brief | Побитовый правый сдвиг числа decimal на заданное число битов
/// shift
/// |
/// @param decimal - число s21_big_decimal
/// @param shift - расстояние сдвига (int)
/// @return
/// число s21_big_decimal
s21_big_decimal rightShiftBigDecimal(s21_big_decimal decimal, int shift) {
  s21_big_decimal result = decimal;
  unsigned int bit_value = 0;
  if (MAX_BIT_POSITION_BIG_DECIMAL - shift < 0) {
    initBigDecimal(&result);
  } else {
    for (int bit_position = 0;
         bit_position <= MAX_BIT_POSITION_BIG_DECIMAL - shift; bit_position++) {
      bit_value = getBitBigDecimal(result, bit_position + shift);
      setBitBigDecimal(&result, 0, bit_position + shift);
      setBitBigDecimal(&result, bit_value, bit_position);
    }
  }
  return result;
}

/// @brief | Возвращает позицию старшего ненулевого бита в числе decimal |
/// @param decimal - число s21_decimal
/// @return
/// -1 - в числе нет ненулевого бита;
/// 0-95 - номер бита
int getFirstNonZeroBitNumber(s21_decimal decimal) {
  int result = -1;
  for (int bit_position = MAX_BIT_POSITION; bit_position >= 0 && result == -1;
       bit_position--) {
    if (getBit(decimal, bit_position)) {
      result = bit_position;
    }
  }
  return result;
}

/// @brief | Возвращает позицию старшего ненулевого бита в числе decimal |
/// @param decimal - число s21_big_decimal
/// @return
/// -1 - в числе нет ненулевого бита;
/// 0-223 - номер бита
int getFirstNonZeroBitNumberBigDecimal(s21_big_decimal decimal) {
  int result = -1;
  for (int bit_position = MAX_BIT_POSITION_BIG_DECIMAL;
       bit_position >= 0 && result == -1; bit_position--) {
    if (getBitBigDecimal(decimal, bit_position)) {
      result = bit_position;
    }
  }
  return result;
}

/// @brief | Возвращает знак числа decimal |
/// @param decimal - число s21_decimal
/// @return
/// 0 - число положительное;
/// 1 - число отрицательное
int getSign(s21_decimal decimal) {
  unsigned int mask = 0x80000000;
  return !!(decimal.bits[SCALE] & mask);
}

/// @brief | Возвращает знак числа decimal |
/// @param decimal - число s21_big_decimal
/// @return
/// 0 - число положительное;
/// 1 - число отрицательное
int getSignBigDecimal(s21_big_decimal decimal) {
  unsigned int mask = 0x80000000;
  return !!(decimal.bits[SCALE_BYTE_BIG_DECIMAL] & mask);
}

/// @brief | Возвращает scale числа decimal |
/// @param decimal - число s21_decimal
/// @return
/// 0-28 - scale числа
unsigned int getScale(s21_decimal decimal) {
  unsigned int mask = 0xFF;
  return (decimal.bits[SCALE] >> 16 & mask);
}

/// @brief | Возвращает scale числа decimal |
/// @param decimal - число s21_big_decimal
/// @return
/// 0-n - scale числа
unsigned int getScaleBigDecimal(s21_big_decimal decimal) {
  unsigned int mask = 0xFF;
  return (decimal.bits[SCALE_BYTE_BIG_DECIMAL] >> 16 & mask);
}

/// @brief | Проверяет является ли число decimal нулем |
/// @param decimal - число s21_decimal
/// @return
/// 0 - FALSE;
/// 1 - TRUE
unsigned int checkDecimalIsZero(s21_decimal decimal) {
  return getFirstNonZeroBitNumber(decimal) == -1;
}

/// @brief | Сложение двух чисел s21_big_decimal |
/// @param value_1 - число s21_big_decimal
/// @param value_2 - число s21_big_decimal
/// @param result - указатель на число s21_big_decimal
void addValuesBigDecimal(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal* result) {
  if (result != NULL) {
    initBigDecimal(result);
    unsigned int scale_value_1 = getScaleBigDecimal(value_1);
    unsigned int scale_value_2 = getScaleBigDecimal(value_2);
    if (scale_value_1 != scale_value_2) {
      if (scale_value_1 < scale_value_2) {
        while (scale_value_1 < scale_value_2) {
          mulBigDecimalBy10(&value_1);
          scale_value_1++;
        }
      } else {
        while (scale_value_2 < scale_value_1) {
          mulBigDecimalBy10(&value_2);
          scale_value_2++;
        }
      }
    }
    unsigned int sign_value_1 = getSignBigDecimal(value_1);
    unsigned int sign_value_2 = getSignBigDecimal(value_2);
    if (sign_value_1) {
      getTwosComplementBigDecimal(value_1, &value_1);
    }
    if (sign_value_2) {
      getTwosComplementBigDecimal(value_2, &value_2);
    }
    unsigned int sign_result = 0;
    unsigned int bit_value_1 = 0;
    unsigned int bit_value_2 = 0;
    unsigned int bit_result = 0;
    unsigned int mask = 0x1;
    for (unsigned int bit_position = 0;
         bit_position <= MAX_BIT_POSITION_BIG_DECIMAL; bit_position++) {
      bit_value_1 = getBitBigDecimal(value_1, bit_position);
      bit_value_2 = getBitBigDecimal(value_2, bit_position);
      bit_result =
          getBitBigDecimal(*result, bit_position) + bit_value_1 + bit_value_2;
      setBitBigDecimal(result, bit_result & mask, bit_position);
      if (bit_position != MAX_BIT_POSITION_BIG_DECIMAL) {
        setBitBigDecimal(result, !!(bit_result & mask << 1), bit_position + 1);
      } else {
        sign_value_1 = getSignBigDecimal(value_1);
        sign_value_2 = getSignBigDecimal(value_2);
        sign_result =
            (sign_value_1 + sign_value_2 + !!(bit_result & mask << 1)) & mask;
        setSignBigDecimal(result, sign_result);
      }
    }
    if (sign_result) {
      getTwosComplementBigDecimal(*result, result);
    }
    setScaleBigDecimal(result, scale_value_1);
  }
}

/// @brief | Вычитание двух чисел s21_big_decimal |
/// @param value_1 - число s21_big_decimal
/// @param value_2 - число s21_big_decimal
/// @param result - указатель на число s21_big_decimal
void subValuesBigDecimal(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal* result) {
  if (result != NULL) {
    unsigned int sign_value_2 = getSignBigDecimal(value_2);
    setSignBigDecimal(&value_2, !sign_value_2);
    addValuesBigDecimal(value_1, value_2, result);
  }
}

/// @brief | Возвращает число src в дополнительном коде dst |
/// @param src - число s21_big_decimal
/// @param dst - указатель на число s21_big_decimal
void getTwosComplementBigDecimal(s21_big_decimal src, s21_big_decimal* dst) {
  if (dst != NULL) {
    initBigDecimal(dst);
    inverseBigDecimal(&src);
    s21_big_decimal additional_unit = {{1, 0, 0, 0, 0, 0, 0, 0}};
    unsigned int sign_src = getSignBigDecimal(src);
    unsigned int sign_dst = 0;
    unsigned int bit_src = 0;
    unsigned int bit_additional_unit = 0;
    unsigned int bit_dst = 0;
    unsigned int mask = 0x1;
    for (unsigned int bit_position = 0;
         bit_position <= MAX_BIT_POSITION_BIG_DECIMAL; bit_position++) {
      bit_src = getBitBigDecimal(src, bit_position);
      bit_additional_unit = getBitBigDecimal(additional_unit, bit_position);
      bit_dst =
          getBitBigDecimal(*dst, bit_position) + bit_src + bit_additional_unit;
      setBitBigDecimal(dst, bit_dst & mask, bit_position);
      if (bit_position != MAX_BIT_POSITION_BIG_DECIMAL) {
        setBitBigDecimal(dst, !!(bit_dst & mask << 1), bit_position + 1);
      } else {
        sign_dst = (sign_src + !!(bit_dst & mask << 1)) & mask;
      }
    }
    setSignBigDecimal(dst, sign_dst);
    unsigned int scale_src = getScaleBigDecimal(src);
    setScaleBigDecimal(dst, scale_src);
  }
}

/// @brief | Побитово инвертирует число src |
/// @param src - указатель на число s21_big_decimal
void inverseBigDecimal(s21_big_decimal* src) {
  if (src != NULL) {
    unsigned int bit_value = 0;
    for (unsigned int bit_position = 0;
         bit_position <= MAX_BIT_POSITION_BIG_DECIMAL; bit_position++) {
      bit_value = getBitBigDecimal(*src, bit_position);
      setBitBigDecimal(src, !bit_value, bit_position);
    }
  }
}

/*
========================================================
                  CHECK FUCTIONs
========================================================
*/

// /// @brief | Печатает побайтово в 16сс число decimal |
// /// @param  decimal - число s21_decimal
// void printDecimal(s21_decimal decimal) {
//   printf("bits[0] = %X\n", decimal.bits[LOW]);
//   printf("bits[1] = %X\n", decimal.bits[MID]);
//   printf("bits[2] = %X\n", decimal.bits[HIGH]);
//   printf("scale = %X\n", decimal.bits[SCALE]);
// }

// /// @brief | Печатает побайтово в 16сс число decimal |
// /// @param  decimal - число s21_big_decimal
// void printBigDecimal(s21_big_decimal decimal) {
//   for (unsigned int num_byte = 0; num_byte < MAX_NUM_OF_BYTES_BIG_DECIMAL;
//        num_byte++) {
//     printf("bits[%u] = %X\n", num_byte, decimal.bits[num_byte]);
//   }
// }

// /// @brief | Печатает побитово число fnum |
// /// @param  fnum - число float
// void printFloatInBites(float fnum) {
//   unsigned int num = 0;
//   memcpy(&num, &fnum, 4);
//   unsigned int mask = 0x80000000;
//   for (int i = 0; i < 32; i++, mask >>= 1) {
//     printf("%u", !!(num & mask));
//   }
//   printf("\n");
// }

// /// @brief | Печатает побитово число decimal |
// /// @param  decimal - число s21_decimal
// void printDecimalInBits(s21_decimal decimal) {
//   printf("scale:  \t");
//   for (int bit_position = MAX_BIT_POSITION + 32; bit_position >= 0;
//        bit_position--) {
//     unsigned int mask = 0x1;
//     if (bit_position >= MAX_BIT_POSITION + 1) {
//       printf("%u", !!(decimal.bits[SCALE] &
//                       mask << bit_position % MAX_NUM_OF_BITS_IN_A_WORD));
//     } else if (bit_position >= 64) {
//       if (bit_position == MAX_BIT_POSITION) {
//         printf("\nbits[3]:\t");
//       }
//       printf("%u", getBit(decimal, bit_position));
//     } else if (bit_position >= 32) {
//       if (bit_position == 63) {
//         printf("\nbits[2]:\t");
//       }
//       printf("%u", getBit(decimal, bit_position));
//     } else {
//       if (bit_position == 31) {
//         printf("\nbits[0]:\t");
//       }
//       printf("%u", getBit(decimal, bit_position));
//     }
//   }
//   printf("\n");
// }

// /// @brief | Печатает побитово число decimal |
// /// @param  decimal - число s21_big_decimal
// void printBigDecimalInBits(s21_big_decimal decimal) {
//   printf("scale:  \t");
//   unsigned int mask = 0x1;
//   for (int bit_position = MAX_BIT_POSITION_BIG_DECIMAL + 32;
//        bit_position > MAX_BIT_POSITION_BIG_DECIMAL; bit_position--) {
//     printf("%u", !!(decimal.bits[SCALE_BYTE_BIG_DECIMAL] &
//                     mask << bit_position % MAX_NUM_OF_BITS_IN_A_WORD));
//   }
//   printf("\nbits[6]:\t");
//   for (int bit_position = MAX_BIT_POSITION_BIG_DECIMAL; bit_position >= 0;
//        bit_position--) {
//     printf("%u", getBitBigDecimal(decimal, bit_position));
//     if (!(bit_position % MAX_NUM_OF_BITS_IN_A_WORD) && bit_position != 0) {
//       printf("\nbits[%d]:\t", bit_position / MAX_NUM_OF_BITS_IN_A_WORD - 1);
//     }
//   }
//   printf("\n");
// }
