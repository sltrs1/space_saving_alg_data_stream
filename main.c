// Задача:
// На вход алгоритма подается файл очень большого размера,
// содержащий по одному слову на каждой строке.
// Необходимо найти TOP-10 самых часто встречающихся слов.
// При реализации надо использовать не более 1 Кб помяти.
// Гаранторовано, что слова достаточно короткие,
// чтобы любые 10 из них могли поместиться в отведенную пома
// Комментарий:
// Реализованный алгоритм был обнаружент мной в публикации
// 'Finding the Frequent Items in Streams of Data' by Cormode, Hadjieleftheriou, 2008.
// Алгоритм работает с ошибкой, обратно пропорциональной
// числу использованных счетчиков epsilon = 1/num_of_counters.
// Алгоритм также может быть использован для бесконечных потоков данных,
// но данная реализация ограничена одинм файлом.
// Алгоритм может быть обобщен для ограниченного лишь доступной памятью
// количества обнаруживаемых слов и для соответсвенно ограниченной точности.
// Для проверки алгоритма были сгенерированы файлы с входными данными
// input_numbers.txt (содержит только числа)
// и input_words.txt (содержит строки из букв латинского алфавита),
// а так же файлы с ответами input_numbers_answer.txt и input_words_answer.txt,
// содержащиеся в этом репозитории.

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include "functions.h"

#define MAX_STR_LEN 20
#define BUF_SIZE 20
#define TOP_N 10 // MUST BE NO MORE THAN BUF_SIZE

int main(int argc, char** argv)
{
    FILE * F = NULL; // Фйал, откуда считываются данные.
    char ** buf; // Буфер, в котором хранятся данные
    size_t cnt[BUF_SIZE]; // Счетчик для каждого элемента буфера, сколько раз он встречен в файле
    size_t overest[BUF_SIZE]; // Переоценки
    size_t i = 0;
    size_t j = 0;
    size_t tmp = 0;
    size_t all_vars_size = 0;

// --------------------------------------------------
// Preparations
    if (argc < 2)
    {
        perror("No file\n");
        return -1;
    }

    F = fopen(argv[1], "r");

    if (F == NULL)
    {
        perror("Cannot open file\n");
        return -2;
    }

    if (TOP_N > BUF_SIZE)
    {
        perror("Cannot find more elements than can fin in memory\n");
        return -3;
    }

    buf = (char**)malloc(sizeof(char*)*BUF_SIZE);
    for (i = 0; i < BUF_SIZE; i++)
    {
        buf[i] = (char*)malloc(sizeof(char)*MAX_STR_LEN);
        memset(buf[i], 0, sizeof(char)*MAX_STR_LEN);
    }
    memset(cnt, 0, sizeof(size_t)*BUF_SIZE);
    memset(overest, 0, sizeof(size_t)*BUF_SIZE);


    all_vars_size = sizeof(F) + sizeof(buf[0][0])*BUF_SIZE*MAX_STR_LEN \
            + sizeof(cnt[0])*BUF_SIZE + sizeof(overest[0])*BUF_SIZE \
            + sizeof(i) + sizeof(j) + sizeof(all_vars_size);
    printf("Size of all user variables is %u bytes.\n", all_vars_size);
// --------------------------------------------------
// Action

    tmp = read_initial_buf(&F, buf, cnt, BUF_SIZE, MAX_STR_LEN);

    if (tmp < BUF_SIZE)
    {
        printf("File too short, fit in memory in full\n");
        sort_buf(buf, cnt, BUF_SIZE);
        tmp = tmp < TOP_N ? tmp : TOP_N;
        print_buf(buf, cnt, tmp);
    }


    while (!feof(F))
    {
        if (get_next(&F, buf, cnt, BUF_SIZE, MAX_STR_LEN) == 2)
        {
            break;
        }
    }

    sort_buf(buf, cnt, BUF_SIZE);

    print_buf(buf, cnt, TOP_N);

// --------------------------------------------------
    fclose(F);
    return 0;
}
