#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  nulling(dst);
  int error = 0;
  if (src < 0) {
    /*Если src отрицательный, вызывается функция set_sign
    для установки знака в структуре dst на отрицательный*/
    set_sign(dst, 1);
    src = -src;
  }

  if (src > INT_MAX)
    error = 1;
  else
    /*Значение src присваивается первому целому числу в массиве bits структуры
     * dst*/
    dst->bits[0] = src;
  return error;
}