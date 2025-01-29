#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  /*Функция s21_truncate возвращает заданный decimal без дробной части.*/
  int error = 0;
  nulling(result);
  /*Обнуляет результат*/
  if (!get_scale(value)) {
    /*Функция get_scale проверяет масштаб заданного decimal.
    Если масштаб равен 0 (что означает, что число целое), то выполняется код
    внутри блока if.*/
    cp_decimal(value, result);
    /*Функция cp_decimal Копирует измененное значение из value в result.
    Это делается для сохранения результата операции обрезки.*/
  } else {
    float float_value = 0.0;
    s21_from_decimal_to_float(value, &float_value);
    /*Преобразует заданное число decimal в число с плавающей точкой, используя
    функцию s21_from_decimal_to_float, и сохраняет результат в float_value.*/
    float_value = trunc(float_value);
    /*Обрезает дробную часть числа float_value, используя функцию trunc.*/
    if (float_value < 0.0) {
      set_sign(result, 1);
      /*Меняет на положительный знак*/
      float_value *= -1;
    }
    for (int i = 0; float_value >= 1 && i < 96; i++) {
      float_value = floorl(float_value) / 2;
      /*Обрезает дробную часть числа float_value и делит его на 2, используя
       * функции floorl и деления.*/
      if (float_value - floorl(float_value) > powl(10.0, -1 * 7)) {
        /*Проверяет, является ли дробная часть числа float_value больше, чем
        10^-7. Если да, то выполняется код внутри блока if.*/
        set_bit(result, i, 1);
        /*Устанавливает бит в позиции i в result в значение 1.*/
      }
    }
    set_scale(result, 0);
    /*Устанавливает масштаб числа в result в 0.*/
  }
  return error;
}