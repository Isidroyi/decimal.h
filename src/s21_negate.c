#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  /*Функция s21_negate возвращает заданный decimal но с противоположным
   * знаком.*/
  int error = 0;
  if (get_sign(value)) {
    /*Функция  get_sign проверяет знак заданного decimal.*/
    set_sign(&value, 0);
    /*Меняет на положительный знак*/
  } else {
    set_sign(&value, 1);
    /*Меняет на отрицательный знак*/
  }
  cp_decimal(value, result);
  /*Функция cp_decimal Копирует измененное значение из value в result.
  Это делается для сохранения результата операции изменения знака.*/
  return error;
}
