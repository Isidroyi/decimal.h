#include "s21_decimal.h"

int s21_is_less_or_equal(s21_decimal val1, s21_decimal val2) {
  /*Функция s21_is_less_or_equal сравнивает два значения типа s21_decimal и
   * возвращает 1, если первое значение меньше или равно второму, или 0, если
   * первое значение больше второго.*/
  int a = s21_is_less(val1, val2);
  /*Функция s21_is_less сравнивает значения val1 и val2 и возвращает 1, если
   * первое значение меньше второго, или 0, если нет.*/
  int b = s21_is_equal(val1, val2);
  return a || b;
  /*Возвращается значение a || b, которое будет 1, если первое значение меньше
   * или равно второму, или 0, если первое значение больше второго.*/
}