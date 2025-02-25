#include "s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int error = 0;
  /*Вызывается функция get_scale для определения масштаба числа*/
  int scale = get_scale(src);
  /*Проверяется, установлены ли биты второго или третьего целого числа в
   src.bits. Если хотя бы один из них установлен, то устанавливается флаг
   ошибки.*/
  if (src.bits[1] || src.bits[2]) {
    error = 1;
  } else {
    /*Значение первого целого числа src.bits присваивается по адресу,
     на который указывает dst*/
    *dst = src.bits[0];
    /*Проверяется, находится ли масштаб scale в допустимом диапазоне от 1
     * до 28.*/
    if (scale > 0 && scale <= 28) {
      /*Если масштаб находится в допустимом диапазоне,
       то значение dst делится на 10 в степени scale, чтобы учесть
       масштабирование.*/
      *dst /= pow(10, scale);
    }
  }
  /*Если установлен 127-й бит числа src, что может указывать на отрицательное
   число, то значение dst умножается на -1, что делает его отрицательным.*/
  if (get_bit(src, 127)) *dst = -*dst;
  return error;
}