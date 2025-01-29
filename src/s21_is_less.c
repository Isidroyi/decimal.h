#include "s21_decimal.h"

int s21_is_less(s21_decimal val1, s21_decimal val2) {
  /*Функция s21_is_less сравнивает два значения типа s21_decimal и возвращает 1,
   * если первое значение меньше второго, или 0, если нет.*/
  big_decimal big1 = {0}, big2 = {0};
  /*big1 и big2: переменные типа big_decimal, используются для хранения значений
   * val1 и val2 в формате big_decimal*/
  decimal_to_big(val1, &big1);
  /*Функция decimal_to_big преобразует значение val1 в формат big_decimal и
   * сохраняет результат в переменной big1*/
  decimal_to_big(val2, &big2);
  int error = big_is_less(big1, big2);
  /*Функция big_is_less сравнивает значения big1 и big2 и возвращает 1, если
   * первое значение меньше второго, или 0, если нет.*/
  return error;
  /*Возвращается значение error, которое будет 1, если первое значение меньше
   * второго, или 0, если нет.*/
}