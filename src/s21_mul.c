#include "s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0, scale = 0;

  big_decimal v1 = {0}, v2 = {0}, r = {0};  // Создаем временные переменные для
                                            // хранения операндов и результата

  // Конвертация операндов в формат с фиксированной точностью с большими числами
  s21_import_to_big_decimal(value_1, &v1);
  s21_import_to_big_decimal(value_2, &v2);

  // Установка знака результата, если операнды имеют разные знаки
  if (get_sign(value_1) != get_sign(value_2))
    set_sign(result, 1);  // Устанавливаем знак результата в положительный

  // Вычисление нового масштаба для результата
  scale = get_scale(value_1) + get_scale(value_2);

  // Выполнение операции умножения с большими числами
  error = s21_mul_big_decimal(v1, v2, &r);

  // Постобработка результата и определение масштаба
  scale = s21_post_normalization(&r, scale);

  // Если масштаб неотрицателен, конвертируем результат обратно в формат с
  // фиксированной точностью с малыми числами
  if (scale >= 0) {
    set_scale(result, scale);  // Устанавливаем масштаб результата
    s21_import_to_small_decimal(result, r);
  } else {
    error = 1;  // Ошибка, если масштаб отрицателен
  }

  // Проверка на ошибки
  if (error == 1 && get_sign(*result))
    error = 2;  // Если результат отрицательный, устанавливаем ошибку 2
  if (error) nulling(result);  // Если есть ошибка, обнуляем результат

  return error;  // Возвращаем код ошибки (0 - успешно, 1 - ошибка, 2 -
                 // неправильный знак)
}
