#include "s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;

  // Проверка на непустое значение делителя
  if (s21_is_decimal_no_empty(value_2)) {
    int scale = 0, res_scale = 0;

    big_decimal v1 = {0}, v2 = {0},
                r = {0};  // Создаем временные переменные для хранения операндов
                          // и результата

    // Конвертация операндов в формат с фиксированной точностью с большими
    // числами
    s21_import_to_big_decimal(value_1, &v1);
    s21_import_to_big_decimal(value_2, &v2);

    // Установка знака результата, если операнды имеют разные знаки
    if (get_sign(value_1) != get_sign(value_2))
      set_sign(result, 1);  // Устанавливаем знак результата в положительный

    // Выполнение операции деления с большими числами
    scale = s21_div_big_decimal(v1, v2, &r);

    // Увеличение масштаба первого операнда на значение масштаба результата
    set_scale(&value_1, get_scale(value_1) + scale);

    // Вычисление нового масштаба результата
    res_scale = get_scale(value_1) - get_scale(value_2);

    // Постобработка результата в зависимости от разности масштабов операндов
    if (res_scale > 0) {
      res_scale = s21_post_normalization(&r, res_scale);
    } else if (res_scale < 0) {
      s21_increase_scale_big_decimal(&r, abs(res_scale));
      res_scale = s21_post_normalization(&r, 0);
    }

    // Если масштаб неотрицателен, конвертируем результат обратно в формат с
    // фиксированной точностью с малыми числами
    if (res_scale >= 0) {
      s21_import_to_small_decimal(result, r);
      set_scale(result, res_scale);  // Устанавливаем масштаб результата
    } else {
      error = 1;  // Ошибка, если масштаб отрицателен
    }
  } else {
    error = 3;  // Ошибка, если делитель равен нулю
  }

  // Проверка на ошибки
  if (error == 1 && get_sign(*result))
    error = 2;  // Если результат отрицательный, устанавливаем ошибку 2
  if (error) nulling(result);  // Если есть ошибка, обнуляем результат

  return error;  // Возвращаем код ошибки (0 - успешно, 1 - ошибка, 2 -
                 // неправильный знак, 3 - деление на ноль)
}
