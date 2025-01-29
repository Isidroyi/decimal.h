#include "s21_decimal.h"

void s21_add_big_decimal(big_decimal value_1, big_decimal value_2,
                         big_decimal *result) {
  int scale = big_get_scale(value_1);
  int res = 0, tmp = 0;
  for (int i = 0; i < 224; i++) {
    res = big_get_bit(value_1, i) + big_get_bit(value_2, i) + tmp;
    tmp = res / 2;
    big_set_bit(result, i, res % 2);
  }
  big_set_scale(result, scale);
}

void s21_sub_big_decimal(big_decimal value_1, big_decimal value_2,
                         big_decimal *result) {
  int tmp = 0, res = 0;
  for (int i = 0; i < 224; i++) {
    res = big_get_bit(value_1, i) - big_get_bit(value_2, i) - tmp;
    if (res < 0) {
      tmp = 1;
    } else {
      tmp = 0;
    }
    res = abs(res);
    big_set_bit(result, i, res % 2);
  }
}

int s21_mul_big_decimal(big_decimal value_1, big_decimal value_2,
                        big_decimal *result) {
  int error = 0, count = 0;
  for (int i = 0; i < 224 && !error; i++) {
    if (big_get_bit(value_2, i)) {
      error = s21_shift_big_dec_left(&value_1, i - count);
      s21_add_big_decimal(value_1, *result, result);
      count = i;
    }
  }
  return error;
}

int s21_div_big_decimal(big_decimal value_1, big_decimal value_2,
                        big_decimal *result) {
  int b_1 = 0, b_2 = 0, bit_2 = 0, scale = 0, diff = 0, save_scale = 0;
  big_decimal tmp = {0}, back_up_result = {0};
  save_scale = big_get_scale(value_1);
  big_set_scale(&value_1, 0);
  s21_find_highest_bit_big_decimal(value_1, value_2, &b_1, &b_2);
  bit_2 = b_2;

  for (int i = 0; i < 96 && s21_is_big_decimal_not_empty(value_1) &&
                  save_scale < 28 && result->bits[3] == 0 &&
                  result->bits[4] == 0 && result->bits[5] == 0;) {
    if (result->bits[3] == 0 && result->bits[4] == 0 && result->bits[5] == 0) {
      back_up_result = *result;
    }

    if (i > 0) {
      s21_increase_scale_big_decimal(result, 1);
      s21_increase_scale_big_decimal(&value_1, 1);
      save_scale++;
    }

    scale = s21_equation_bits_big_decimal(&value_1, &value_2);
    save_scale += scale;
    b_1 = b_2 = 0;
    s21_find_highest_bit_big_decimal(value_1, value_2, &b_1, &b_2);
    diff = b_2 - bit_2;
    if (diff < 0) diff = 0;

    for (; diff >= 0 && s21_is_big_decimal_not_empty(value_1) &&
           tmp.bits[3] == 0 && tmp.bits[4] == 0 && tmp.bits[5] == 0 &&
           tmp.bits[6] == 0;) {
      if (s21_is_greater_big_decimal(value_2, value_1)) {
        big_set_bit(&tmp, 0, 0);
      } else {
        s21_sub_big_decimal(value_1, value_2, &value_1);
        big_set_bit(&tmp, 0, 1);
      }

      i++;
      diff--;
      if (diff >= 0) s21_shift_big_dec_right(&value_2, 1);
      s21_shift_big_dec_left(&tmp, 1);

      if (tmp.bits[3] != 0 || tmp.bits[4] != 0 || tmp.bits[5] != 0 ||
          tmp.bits[6] != 0) {
        break;
      }
    }

    if (diff >= 0) {
      s21_shift_big_dec_left(&tmp, diff + 1);
    }
    s21_shift_big_dec_right(&tmp, 1);
    s21_add_big_decimal(*result, tmp, result);
    s21_zero_big_decimal(&tmp);
  }

  if (result->bits[3] != 0 || result->bits[4] != 0 || result->bits[5] != 0) {
    *result = back_up_result;
    save_scale--;
  }

  return save_scale;
}

void s21_import_to_big_decimal(s21_decimal value_1, big_decimal *value_2) {
  value_2->bits[0] = value_1.bits[0];
  value_2->bits[1] = value_1.bits[1];
  value_2->bits[2] = value_1.bits[2];
}

void s21_import_to_small_decimal(s21_decimal *value_1, big_decimal value_2) {
  value_1->bits[0] = value_2.bits[0];
  value_1->bits[1] = value_2.bits[1];
  value_1->bits[2] = value_2.bits[2];
}

void big_set_bit(big_decimal *dst, int index, int bit) {
  int mask = 1u << (index % 32);
  if (bit == 0)
    dst->bits[index / 32] = dst->bits[index / 32] & ~mask;
  else
    dst->bits[index / 32] = dst->bits[index / 32] | mask;
}

int big_get_bit(big_decimal dst, int index) {
  int mask = 1u << (index % 32);
  return (dst.bits[index / 32] & mask) != 0;
}

void s21_shift_big_dec_right(big_decimal *dst, int num) {
  while (num--) {
    for (int i = 0; i < 7; i++) {
      int num_int = 32 * 1 + 32 * i;
      int buff = big_get_bit(*dst, num_int);
      dst->bits[i] >>= 1;
      big_set_bit(dst, num_int - 1, buff);
    }
  }
}

int s21_shift_big_dec_left(big_decimal *val, int num) {
  int error = 0;
  while (num--) {
    for (int i = 6; i >= 0; i--) {
      int num_int = 32 * 1 + 32 * i - 1;
      int buff = big_get_bit(*val, num_int);
      val->bits[i] <<= 1;
      big_set_bit(val, num_int + 1, buff);
    }
  }
  return error;
}

void s21_normalization(big_decimal *value_1, big_decimal *value_2, int diff) {
  int sign1 = big_get_sign(*value_1);
  int sign2 = big_get_sign(*value_2);

  big_set_sign(value_1, 0);
  big_set_sign(value_2, 0);
  if (diff > 0) {
    s21_increase_scale_big_decimal(value_2, diff);
  } else if (diff < 0) {
    s21_increase_scale_big_decimal(value_1, -diff);
  }
  big_set_sign(value_1, sign1);
  big_set_sign(value_2, sign2);
}

int s21_post_normalization(big_decimal *result, int scale) {
  int dop = 0;

  while ((result->bits[3] != 0 || result->bits[4] != 0 ||
          result->bits[5] != 0 || result->bits[6] != 0) &&
         scale > 0) {
    if (scale == 1 && result->bits[3]) {
      dop = 1;
    }

    s21_decreace_scale_big_decimal(result, 1);
    scale--;
  }

  if (dop && scale == 0 && result->bits[3] == 0 && big_get_bit(*result, 0))
    big_set_bit(result, 0, 0);
  if ((result->bits[3] || result->bits[4] || result->bits[5] ||
       result->bits[6]))
    scale = -1;
  return scale;
}

void s21_zero_big_decimal(big_decimal *dst) {
  dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = dst->bits[4] =
      dst->bits[5] = dst->bits[6] = dst->bits[7] = 0;
}

void s21_increase_scale_decimal(s21_decimal *dst, int n) {
  s21_decimal tmp = {0}, ten = {{10, 0, 0, 0}};
  int scale = get_scale(*dst);
  for (int i = 0; i < n; i++) {
    s21_mul(*dst, ten, &tmp);
    *dst = tmp;
    nulling(&tmp);
  }
  set_scale(dst, scale + n);
}

void s21_increase_scale_big_decimal(big_decimal *dst, int n) {
  big_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}}, tmp = {0};
  int scale = big_get_scale(*dst);
  for (int i = 0; i < n; i++) {
    s21_mul_big_decimal(*dst, ten, &tmp);
    *dst = tmp;
    s21_zero_big_decimal(&tmp);
  }
  big_set_scale(dst, scale);
}

void s21_decreace_scale_big_decimal(big_decimal *dst, int n) {
  big_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}}, tmp = {0};

  for (int i = 0; i < n; i++) {
    s21_div_big_for_decrease_to_decimal(*dst, ten, &tmp);
    *dst = tmp;
    s21_zero_big_decimal(&tmp);
  }
}

int s21_is_greater_big_decimal(big_decimal value_1, big_decimal value_2) {
  /*Функция s21_is_greater_big_decimal сравнивает два значения типа big_decimal
   * и возвращает 1, если первое значение больше второго, или 0, если нет.*/
  int result = 0, out = 0;
  /*result используется для хранения результата сравнения (1 - первое больше, 0
  - нет) out используется как флаг выхода из цикла (1 - выход, 0 -
  продолжение).*/
  for (int i = 7; i >= 0 && !result && !out; i--) {
    /*Внутри цикла проверяется, установлен ли хотя бы один бит в соответствующих
     * позициях обоих значений (value_1.bits[i] или value_2.bits[i])*/
    if (value_1.bits[i] || value_2.bits[i]) {
      if (value_1.bits[i] > value_2.bits[i]) {
        result = 1;
        /*Если бит в value_1 установлен, а в value_2 нет (бит value_1 выше),
         * устанавливается result в 1 (первое больше)*/
      }
      if (value_1.bits[i] != value_2.bits[i]) out = 1;
      /*Если биты отличаются (не оба нуля и не оба единицы), устанавливается out
       * в 1 для выхода из цикла (не нужно дальше сравнивать)*/
    }
  }
  return result;
}

int s21_is_greater_or_equal_big_decimal(big_decimal value_1,
                                        big_decimal value_2) {
  int result = 0, out = 0;
  for (int i = 7; i >= 0 && !out && !result; i--) {
    if (value_1.bits[i] != 0 || value_2.bits[i] != 0) {
      if (value_1.bits[i] >= value_2.bits[i]) {
        result = 1;
      }
      out = 1;
    }
  }
  return result;
}

int s21_is_decimal_no_empty(s21_decimal dst) {
  return dst.bits[0] + dst.bits[1] + dst.bits[2];
}

int s21_is_big_decimal_not_empty(big_decimal dst) {
  int error = 0;
  int a, b;
  big_decimal null = {0};

  for (int i = 0; i < 224 && error == 0; i++) {
    a = big_get_bit(dst, i);
    b = big_get_bit(null, i);
    if (a != b) {
      error = 1;
    }
  }
  return error;
}

void s21_find_highest_bit_decimal(s21_decimal v1, s21_decimal v2, int *bit_1,
                                  int *bit_2) {
  for (int i = 95; i >= 0 && (!(*bit_1) || !(*bit_2)); i--) {
    if (*bit_1 == 0 && get_bit(v1, i)) *bit_1 = i;
    if (*bit_2 == 0 && get_bit(v2, i)) *bit_2 = i;
  }
}

void s21_find_highest_bit_big_decimal(big_decimal v1, big_decimal v2,
                                      int *bit_1, int *bit_2) {
  for (int i = 223; i >= 0 && (!(*bit_1) || !(*bit_2)); i--) {
    if (*bit_1 == 0 && big_get_bit(v1, i)) *bit_1 = i;
    if (*bit_2 == 0 && big_get_bit(v2, i)) *bit_2 = i;
  }
}

int s21_equation_bits_big_decimal(big_decimal *value_1, big_decimal *value_2) {
  int scale = 0;

  while (s21_is_greater_big_decimal(*value_2, *value_1)) {
    s21_increase_scale_big_decimal(value_1, 1);
    scale++;
  }
  while (s21_is_greater_or_equal_big_decimal(*value_1, *value_2)) {
    s21_shift_big_dec_left(value_2, 1);
  }

  s21_shift_big_dec_right(value_2, 1);

  return scale;
}

int get_bit(s21_decimal val, int index) {
  /* Эта строка объявляет переменную num_bit
  и присваивает ей значение остатка от деления index на 32.
  Это нужно для определения номера бита внутри одного из целых чисел. */
  int num_bit = index % 32;
  /* присваиваем ей результат целочисленного деления index на 32.
   Это определяет, в каком целом числе находится бит, который мы хотим
   получить.*/
  int num_int = index / 32;

  /*val.bits[num_int] - это обращение к num_int-ому целому числу в структуре
  val. & (1 << num_bit) - это побитовая операция "И" между num_bit-ым битом и 1,
  который сдвинут на num_bit разрядов влево.
  Это помогает выделить нужный бит.
  >> num_bit - это сдвиг вправо на num_bit разрядов. Это делается для того,
  чтобы нужный бит стал младшим битом числа.
  Таким образом, в итоге функция возвращает значение одного бита из числа
  val.bits[num_int]*/
  return (val.bits[num_int] & (1 << num_bit)) >> num_bit;
}

int get_scale(s21_decimal val) {
  int scale = 0;
  for (int i = 96 + 23; i >= 96 + 16; i--) {
    /*Здесь происходит сдвиг битов переменной scale на один бит влево.
    Это эквивалентно умножению на два,
    и это делается для подготовки места для следующего бита.*/
    scale <<= 1;
    /*В этой строке мы используем побитовую операцию "ИЛИ с присваиванием" (|=).
    Мы присваиваем scale результат побитового ИЛИ между текущим значением scale
    и результатом вызова функции get_bit для числа val и индекса i.
    Это позволяет установить нужный бит в scale.*/
    scale |= get_bit(val, i);
  }
  return scale;
}

void set_bit(s21_decimal *val, int index, int bit) {
  int num_int = index / 32;
  int num_bit = index % 32;
  if (bit == 1) {
    val->bits[num_int] |= (1 << num_bit);
  } else {
    val->bits[num_int] &= (~(1 << num_bit));
  }
}

void set_scale(s21_decimal *val, int scale) {
  for (int i = 96 + 16; i <= 96 + 23; i++) {
    set_bit(val, i, 0);
  }
  int mask = 1 << 16;
  scale <<= 16;

  for (int i = 96 + 16; i <= 96 + 23; i++) {
    if (scale & mask) {
      set_bit(val, i, 1);
    }
    mask <<= 1;
  }
}

void set_sign(s21_decimal *val, int sign) { set_bit(val, 127, sign); }

void big_set_sign(big_decimal *val, int sign) { big_set_bit(val, 255, sign); }

int get_sign(s21_decimal val) { return get_bit(val, 127); }

void nulling(s21_decimal *val) {
  for (int i = 0; i < 4; i++) {
    /* значение 0 присваивается i-тому целому числу в массиве bits, который
    находится в структуре, на которую указывает указатель val. Это обнуляет все
    четыре целых числа в структуре s21_decimal.*/
    val->bits[i] = 0;
  }
}

void decimal_to_big(s21_decimal val1, big_decimal *val2) {
  val2->bits[7] = val1.bits[3];
  for (int i = 0; i < 3; i++) {
    val2->bits[i] = val1.bits[i];
  }
}

void big_set_scale(big_decimal *val, int scale) {
  for (int i = 224 + 16; i <= 224 + 23; i++) {
    big_set_bit(val, i, 0);
  }
  int mask = 1 << 16;
  scale <<= 16;

  for (int i = 224 + 16; i <= 224 + 23; i++) {
    if (scale & mask) {
      big_set_bit(val, i, 1);
    }
    mask <<= 1;
  }
}

int big_get_scale(big_decimal val) {
  int scale = 0;
  for (int i = 224 + 23; i >= 224 + 16; i--) {
    scale <<= 1;
    scale |= big_get_bit(val, i);
  }
  return scale;
}

void cp_decimal(s21_decimal src, s21_decimal *dst) {
  nulling(dst);
  for (int i = 0; i < 4; i++) {
    dst->bits[i] = src.bits[i];
  }
}

int big_is_equal(big_decimal value_1, big_decimal value_2) {
  int sign1 = big_get_sign(value_1);
  int sign2 = big_get_sign(value_2);
  int error = 1;
  int a, b;
  int diff = big_get_scale(value_1) - big_get_scale(value_2);
  s21_normalization(&value_1, &value_2, diff);

  // big_equling_scale(&value_1, &value_2);
  if (big_null(value_1) && big_null(value_2)) {
    error = 1;
  } else {
    for (int i = 0; i < 224 && error == 1; i++) {
      a = big_get_bit(value_1, i);
      b = big_get_bit(value_2, i);
      if (a != b) {
        error = 0;
      }
    }
    if (sign1 != sign2 && error == 1) {
      error = 0;
    }
  }

  return error;
}

int big_is_less(big_decimal val1, big_decimal val2) {
  // Возвращаемое значение:
  // - 0 - FALSE
  // - 1 - TRUE
  int error = 0;
  int bit1 = 0;
  int bit2 = 0;
  int sign1 = big_get_sign(val1);
  int sign2 = big_get_sign(val2);
  int zero1 = big_null(val1);
  int zero2 = big_null(val2);

  if (zero1 == 1 && zero2 == 1) {
    error = 0;
  } else {
    int diff = big_get_scale(val1) - big_get_scale(val2);
    s21_normalization(&val1, &val2, diff);

    if (sign1 == 1 && sign2 == 0) {
      error = 1;
    } else if (sign1 == 0 && sign2 == 1) {
      error = 0;
    } else if (sign1 == sign2) {
      for (int i = 223; i >= 0; i--) {
        bit1 = big_get_bit(val1, i);
        bit2 = big_get_bit(val2, i);
        if (bit1 != bit2) {
          if (bit1 < bit2) {
            error = 1;
            if (sign1) {
              error = 0;
            }
          } else if (bit1 > bit2) {
            error = 0;
            if (sign1) {
              error = 1;
            }
          }
          break;
        }
      }
    }
  }

  return error;
}

int big_null(big_decimal val) {
  int error = 1;
  big_decimal null = {0};
  int a, b;
  for (int i = 0; i < 224 && error == 1; i++) {
    a = big_get_bit(val, i);
    b = big_get_bit(null, i);
    if (a != b) {
      error = 0;
    }
  }
  return error;
}

int big_get_sign(big_decimal val) { return big_get_bit(val, 255); }

int s21_from_decimal_to_double(s21_decimal src, long double *dst) {
  long double temp = (double)*dst;
  for (int i = 0; i < 96; i++) {
    temp += get_bit(src, i) * pow(2, i);
  }
  temp = temp * pow(10, -get_scale(src));
  if (get_bit(src, 127)) temp = -temp;
  *dst = temp;
  return 0;
}

int s21_div_big_for_decrease_to_decimal(big_decimal value_1,
                                        big_decimal value_2,
                                        big_decimal *result) {
  int b_1 = 0, b_2 = 0, bit_2 = 0, scale = 0, diff = 0, save_scale = 0;
  big_decimal tmp = {0};
  save_scale = big_get_scale(value_1);
  big_set_scale(&value_1, 0);
  s21_find_highest_bit_big_decimal(value_1, value_2, &b_1, &b_2);
  bit_2 = b_2;

  for (int i = 0; i < 96 && s21_is_greater_big_decimal(value_1, value_2) &&
                  save_scale < 28;) {
    if (i > 0) {
      s21_increase_scale_big_decimal(result, 1);
      s21_increase_scale_big_decimal(&value_1, 1);
      save_scale++;
    }

    scale = s21_equation_bits_big_decimal(&value_1, &value_2);
    save_scale += scale;
    b_1 = b_2 = 0;
    s21_find_highest_bit_big_decimal(value_1, value_2, &b_1, &b_2);
    diff = b_2 - bit_2;
    if (diff < 0) diff = 0;

    for (; diff >= 0 && s21_is_big_decimal_not_empty(value_1) &&
           tmp.bits[3] == 0 && tmp.bits[4] == 0 && tmp.bits[5] == 0 &&
           tmp.bits[6] == 0;) {
      if (s21_is_greater_big_decimal(value_2, value_1)) {
        big_set_bit(&tmp, 0, 0);
      } else {
        s21_sub_big_decimal(value_1, value_2, &value_1);
        big_set_bit(&tmp, 0, 1);
      }

      i++;
      diff--;
      if (diff >= 0) s21_shift_big_dec_right(&value_2, 1);
      s21_shift_big_dec_left(&tmp, 1);

      if (tmp.bits[3] != 0 || tmp.bits[4] != 0 || tmp.bits[5] != 0 ||
          tmp.bits[6] != 0) {
        break;
      }
    }

    if (diff >= 0) {
      s21_shift_big_dec_left(&tmp, diff + 1);
    }
    s21_shift_big_dec_right(&tmp, 1);
    s21_add_big_decimal(*result, tmp, result);
    s21_zero_big_decimal(&tmp);
  }

  return save_scale;
}
