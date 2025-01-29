#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  /*Функция s21_floor возвращает заданный decimal окргуленный в меньшую сторону
   * до целого числа.*/
  int error = 0;
  nulling(result);
  /*обнуляет результат*/
  if (!get_scale(value)) {
    /*Проверяет масштаб числа value, используя функцию get_scale.
    Если масштаб равен 0 (что означает, что число целое), то выполняется код
    внутри блока if.*/
    cp_decimal(value, result);
    /*Функция cp_decimal Копирует измененное значение из value в result.
    Это делается для сохранения результата операции округления.*/
  } else {
    float float_value = 0.0;
    s21_from_decimal_to_float(value, &float_value);
    /*Функция s21_from_decimal_to_float преобразует число value в число с
     * плавающей точкой и сохраняет результат в float_value.*/
    float_value = floorf(float_value);
    /*Округляет число float_value вниз до ближайшего целого числа, используя
     * функцию floorf.*/
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
        set_bit(result, i, 1);
        /*Устанавливает бит в позиции i в result в значение 1.*/
      }
    }
    set_scale(result, 0);
    /*Устанавливает масштаб числа в result в 0*/
  }

  return error;
}
