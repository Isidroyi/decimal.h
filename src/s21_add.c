#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0, get = 0, sign_res = 0;

  // Обнуление результата
  nulling(result);

  // Проверка знаков обоих операндов
  if (get_sign(value_1) && get_sign(value_2)) {
    sign_res = 1;  // Если оба операнда положительные, устанавливаем флаг
                   // положительного результата
  }

  // Если операнды имеют разные знаки, выполняем операцию вычитания
  if (get_sign(value_1) != get_sign(value_2)) {
    int sign = 0;
    sign = get_sign(value_1);
    set_bit(&value_1, 127,
            0);  // Установка бита знака в 0 для получения абсолютного значения
    set_bit(&value_2, 127, 0);
    error = sign ? s21_sub(value_2, value_1,
                           result)  // Вычитаем меньшее из большего
                 : s21_sub(value_1, value_2, result);
  } else {
    // Если операнды имеют одинаковый знак, выполняем операцию сложения

    big_decimal v1 = {0}, v2 = {0},
                r = {0};  // Создаем временные переменные для хранения операндов
                          // и результата
    int scale = 0;

    // Вычисление разницы в масштабах операндов
    int diff = get_scale(value_1) - get_scale(value_2);

    if (diff > 0) {
      get = get_scale(value_1);
      set_scale(&value_2, get);  // Приводим второй операнд к масштабу первого
    } else {
      get = get_scale(value_2);
      set_scale(&value_1, get);  // Приводим первый операнд к масштабу второго
    }

    // Конвертация операндов в формат с фиксированной точностью с большими
    // числами
    decimal_to_big(value_1, &v1);
    decimal_to_big(value_2, &v2);

    // Нормализация операндов перед сложением
    s21_normalization(&v1, &v2, diff);

    // Выполнение операции сложения с большими числами
    s21_add_big_decimal(v1, v2, &r);

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
