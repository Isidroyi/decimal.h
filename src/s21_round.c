#include "s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  /*Функция s21_round возвращает заданный decimal округленный до ближайшего
   * целого числа.*/
  nulling(result);
  /*обнуляет результат*/
  long double fl_to_round = 0.0;
  long double difference = 0;
  s21_from_decimal_to_double(value, &fl_to_round);
  /*Функция s21_from_decimal_to_double Преобразует число value в число с
  плавающей точкой, используя функцию s21_from_decimal_to_double, и сохраняет
  результат в fl_to_round.*/
  fl_to_round = round(fl_to_round);
  /*Округляет число fl_to_round до ближайшего целого числа, используя функцию
   * round.*/
  if (fl_to_round < 0.0) {
    set_bit(result, 127, 1);
    /*Устанавливает знак числа в result в положительный*/
    fl_to_round *= -1;
  }
  for (int i = 0; fl_to_round >= 1 && i < 96; i++) {
    fl_to_round = floor(fl_to_round) / 2;
    /*Обрезает дробную часть числа float_value и делит его на 2, используя
     * функции floorl и деления.*/
    difference = fl_to_round - floor(fl_to_round);
    /*Вычисляет разницу между округленным числом и его целой частью.*/
    if (difference > 0.0) {
      set_bit(result, i, 1);
      /*Устанавливает бит в позиции i в result в значение 1*/
    } else {
      set_bit(result, i, 0);
      /*Устанавливает бит в позиции i в result в значение 0*/
    }
  }
  set_scale(result, 0);
  /*Устанавливает масштаб числа в result в 0*/
  return 0;
}
