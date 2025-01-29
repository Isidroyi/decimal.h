#include "s21_decimal.h"

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  /*Функция s21_is_equal сравнивает два значения типа s21_decimal и возвращает
   * 1, если они равны, или 0, если нет.*/
  int error = 1;
  /*error  используется для хранения кода ошибки.*/
  big_decimal big1 = {0}, big2 = {0};
  /*big1 и big2: переменные типа big_decimal, используются для хранения значений
   * value_1 и value_2 в формате big_decimal*/
  decimal_to_big(value_1, &big1);
  /*Функция decimal_to_big преобразует значение value_1 в формат big_decimal и
   * сохраняет результат в переменной big1.*/
  decimal_to_big(value_2, &big2);
  error = big_is_equal(big1, big2);
  /*Функция big_is_equal сравнивает значения big1 и big2 и возвращает 1, если
   * они равны, или 0, если нет.*/
  return error;
}