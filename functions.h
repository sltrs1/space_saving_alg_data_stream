#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stddef.h>
#include <stdio.h>

// Печать буфера
void print_buf(char ** buf, size_t * cnt, size_t buf_size);

// Замена символа конца строки на нулевой символ
// Сделано для удобства печати
void remove_eol(char * str);

// Поиск значения в буфере.
// Возвращает позицию найденой строки в буфере
// или значение, большее размера буфера, в случае ошибки
size_t find_in_buf(char ** buf, char * str, size_t buf_size, size_t max_str_len);

// Инициализация буфера.
// Считывает из файла строки до тех пор, пока буфер не заполнится.
// Возвращает количество помещенных в буфер элементов
// или значение, большее размера буфера, в случае ошибки.
size_t read_initial_buf(FILE ** F, char ** buf, size_t * cnt, size_t buf_size, size_t max_str_len);

// Считать из файла следующую строку и выполнить итерацию алгоритма
size_t get_next(FILE ** F, char ** buf, size_t * cnt, size_t * overest, size_t buf_size, size_t max_str_len);

// Найти индекс первого самого редкого элемента в буфере
size_t find_min(size_t * cnt, size_t buf_size);

// Смена местами двух строк
void swap_buffer_elements(char ** str1_ptr, char ** str2_ptr, size_t * x, size_t * y);

// Сортировка буфера
// Так как сортируется заведомо небольшое количество элементов,
// вместо быстрой сортировка используется простая пузырьковая.
void sort_buf(char ** buf, size_t * cnt, size_t buf_size);

#endif // FUNCTIONS_H
