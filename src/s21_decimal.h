#ifndef TEST
#define TEST

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_DECIMAL 79228162514264337593543950336.0
#define MIN_DECIMAL -79228162514264337593543950336.0

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[8];
} big_decimal;

typedef union {
  int ui;
  float fl;
} floatbits;

// Основные вспомогательные функции
int get_bit(s21_decimal val, int index);
void set_bit(s21_decimal *val, int index, int bit);
int get_scale(s21_decimal val);
void set_scale(s21_decimal *val, int scale);
void set_sign(s21_decimal *val, int sign);
int get_sign(s21_decimal val);

// Преобразователи
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Арифметические операторы
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// Другие функции
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

// Операторы сравнение
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_less(s21_decimal val1, s21_decimal val2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal val1, s21_decimal val2);
int s21_is_greater(s21_decimal val1, s21_decimal val2);
int s21_is_greater_or_equal(s21_decimal val1, s21_decimal val2);

// Вспомогательные функции big

void s21_add_big_decimal(big_decimal value_1, big_decimal value_2,
                         big_decimal *result);
void s21_sub_big_decimal(big_decimal value_1, big_decimal value_2,
                         big_decimal *result);
int s21_mul_big_decimal(big_decimal value_1, big_decimal value_2,
                        big_decimal *result);
int s21_div_big_decimal(big_decimal value_1, big_decimal value_2,
                        big_decimal *result);

void s21_import_to_big_decimal(s21_decimal value_1, big_decimal *value_2);
void s21_import_to_small_decimal(s21_decimal *value_1, big_decimal value_2);

void big_set_bit(big_decimal *dst, int index, int bit);
int big_get_bit(big_decimal dst, int index);

void s21_shift_big_dec_right(big_decimal *dst, int num);

void s21_normalization(big_decimal *value_1, big_decimal *value_2, int diff);
int s21_post_normalization(big_decimal *result, int scale);
void s21_zero_big_decimal(big_decimal *dst);
void s21_increase_scale_decimal(s21_decimal *dst, int n);
void s21_increase_scale_big_decimal(big_decimal *dst, int n);
void s21_decreace_scale_big_decimal(big_decimal *dst, int n);
int s21_is_greater_big_decimal(big_decimal value_1, big_decimal value_2);
int s21_is_greater_or_equal_big_decimal(big_decimal value_1,
                                        big_decimal value_2);
int s21_is_decimal_no_empty(s21_decimal dst);
int s21_is_big_decimal_not_empty(big_decimal dst);
void s21_find_highest_bit_decimal(s21_decimal v1, s21_decimal v2, int *bit_1,
                                  int *bit_2);
void s21_find_highest_bit_big_decimal(big_decimal v1, big_decimal v2,
                                      int *bit_1, int *bit_2);
int s21_equation_bits_big_decimal(big_decimal *value_1, big_decimal *value_2);
void nulling(s21_decimal *val);
void decimal_to_big(s21_decimal val1, big_decimal *val2);
void big_set_scale(big_decimal *val, int scale);
int big_get_scale(big_decimal val);
void cp_decimal(s21_decimal src, s21_decimal *dst);
int big_is_equal(big_decimal value_1, big_decimal value_2);
int big_is_less(big_decimal val1, big_decimal val2);
int big_null(big_decimal val);
int big_get_sign(big_decimal val);
int s21_from_decimal_to_double(s21_decimal src, long double *dst);
void big_set_sign(big_decimal *val, int sign);
int s21_shift_big_dec_left(big_decimal *val, int num);
int s21_div_big_for_decrease_to_decimal(big_decimal value_1,
                                        big_decimal value_2,
                                        big_decimal *result);

#endif  // TEST
