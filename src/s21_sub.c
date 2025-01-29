#include "s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0, scale = 0, sign_res = 0;

  // Обнуление результата
  nulling(result);

  // Проверка знаков обоих операндов
  if (get_sign(value_1) && get_sign(value_2)) {
    // Если оба операнда отрицательные, меняем их местами и приводим к
    // абсолютному значению
    s21_decimal temporary1 = value_1;
    value_1 = value_2;
    value_2 = temporary1;
    set_bit(&value_1, 127, 0);
    set_bit(&value_2, 127, 0);
  }

  // Если операнды имеют разные знаки, выполняем операцию сложения с приведением
  // к абсолютному значению
  if (get_sign(value_1) != get_sign(value_2)) {
    get_sign(value_1) ? sign_res = 1
                      : 1;  // Если первый операнд отрицательный, устанавливаем
                            // флаг положительного результата
    set_bit(&value_1, 127, 0);  // Приводим оба операнда к абсолютному значению
    set_bit(&value_2, 127, 0);
    error = s21_add(value_1, value_2, result);  // Выполняем операцию сложения
  } else {
    // Если операнды имеют одинаковый знак, выполняем операцию вычитания

    big_decimal v1 = {0}, v2 = {0},
                r = {0};  // Создаем временные переменные для хранения операндов
                          // и результата

    // Конвертация операндов в формат с фиксированной точностью с большими
    // числами
    s21_import_to_big_decimal(value_1, &v1);
    s21_import_to_big_decimal(value_2, &v2);

    // Вычисление разницы в масштабах операндов
    int diff = get_scale(value_1) - get_scale(value_2);

    // Приведение операндов к одному масштабу
    diff > 0 ? set_scale(&value_2, get_scale(value_2) + diff)
             : set_scale(&value_1, get_scale(value_1) - diff);

    // Нормализация операндов перед вычитанием
    s21_normalization(&v1, &v2, diff);

    // Если второй операнд больше первого, меняем их местами и устанавливаем
    // знак результата
    if (s21_is_greater_big_decimal(v2, v1)) {
      big_decimal temporary2 = v1;
      v1 = v2;
      v2 = temporary2;
      set_sign(result, 1);
    }

    // Выполнение операции вычитания с большими числами
    s21_sub_big_decimal(v1, v2, &r);

    // Постобработка результата и определение масштаба
    scale = s21_post_normalization(&r, get_scale(value_1));

    if (scale >= 0) {
      // Если масштаб неотрицателен, конвертируем результат обратно в формат с
      // фиксированной точностью с малыми числами
      s21_import_to_small_decimal(result, r);
      set_scale(result, scale);  // Устанавливаем масштаб результата
    } else {
      error = 1;  // Ошибка, если масштаб отрицателен
    }
  }

  // Установка знака результата в зависимости от флага положительного результата
  sign_res == 1 ? set_sign(result, 1) : 0;

  // Проверка на ошибки
  if (error == 1 && get_sign(*result))
    error = 2;  // Если результат отрицательный, устанавливаем ошибку 2
  if (error) nulling(result);  // Если есть ошибка, обнуляем результат

  return error;  // Возвращаем код ошибки (0 - успешно, 1 - ошибка, 2 -
                 // неправильный знак)
}
