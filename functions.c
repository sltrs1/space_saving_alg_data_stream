#include "functions.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

//****************************************************************************
//****************************************************************************

void print_buf(char ** buf, size_t * cnt, size_t buf_size)
{
    if (buf == NULL || cnt == NULL || buf_size == 0)
    {
        perror("print_buf: Invalid parameters\n");
        return;
    }

    size_t i = 0;
    for (i = 0; i < buf_size; i++)
    {
        printf("%u - %s - %u\n", i+1, &buf[i][0], cnt[i]);
    }
}

//****************************************************************************
//****************************************************************************

void remove_eol(char * str)
{
    if (str == NULL)
    {
        perror("remove_eol: Invalid parameters\n");
        return;
    }

    size_t i = 0;

    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] == '\n')
        {
            str[i] = 0;
            break;
        }
    }

}

//****************************************************************************
//****************************************************************************

size_t find_in_buf(char ** buf, char * str, size_t buf_size, size_t max_str_len)
{
    size_t i;
    size_t num_found = buf_size + 1;

    if (buf == NULL || str == NULL || buf_size == 0 || max_str_len == 0 )
    {
        perror("find_in_buf: Invalid parameters\n");
        return num_found;
    }

    for (i = 0; i < buf_size; i++)
    {
        if ( !strncmp(buf[i], str, max_str_len) )
        {
            num_found = i;
            break;
        }
    }

    return num_found;
}

//****************************************************************************
//****************************************************************************

size_t read_initial_buf(FILE ** F, char ** buf, size_t * cnt, size_t buf_size, size_t max_str_len)
{
    size_t counter = buf_size + 1; // Количество считанных строк
    size_t position = 0;
    char * curr_str = NULL;

    if (*F == NULL || buf == NULL || cnt == NULL || buf_size == 0 || max_str_len == 0)
    {
        perror("read_initial_buf: Invalid parameters\n");
        return counter;
    }

    curr_str = (char*)malloc(sizeof(char)*max_str_len);
    counter = 0;

    while(counter < buf_size && !feof(*F) )
    {
        if ( !fgets(curr_str, max_str_len, *F) )
        {
            printf("errno = %i\n", errno);
            printf("ferror = %i\n", ferror(*F));
            break;
        }

        remove_eol(curr_str);
        position = find_in_buf(buf, curr_str, buf_size, max_str_len);

        if ( position > buf_size )
        {
            // Если строка не найдена в буфере, то добавить ее в буфер,
            // инициализировать счетчик элемента и сдвинуть счетчик буфера.
            memcpy(&buf[counter][0], curr_str, strlen(curr_str));
            cnt[counter] = 1;
            counter += 1;
        }
        else
        {
            // Если строка найдена в буфере, то увеличить счетчик
            cnt[position] += 1;
        }

    }
    free(curr_str);

    return counter;
}

//****************************************************************************
//****************************************************************************

void swap_buffer_elements(char ** str1_ptr, char ** str2_ptr, size_t * x, size_t * y)
{
    char * tmp = *str1_ptr;
    *str1_ptr = *str2_ptr;
    *str2_ptr = tmp;

    *x = *x + *y;
    *y = *x - *y;
    *x = *x - *y;

}

//****************************************************************************
//****************************************************************************

void sort_buf(char ** buf, size_t * cnt, size_t buf_size)
{
    if (buf == NULL || cnt == NULL || buf_size == 0)
    {
        perror("sort_buf: Invalid parameters\n");
        return;
    }

    size_t i = 0;
    size_t j = 0;

    for (i = 0; i < buf_size; i++)
    {
        for (j = buf_size - 1; j > i; j--)
        {
            if (cnt[j-1] < cnt[j])
            {
                swap_buffer_elements(&buf[j-1], &buf[j], &cnt[j-1], &cnt[j]);
            }
        }
    }

    return;
}

//****************************************************************************
//****************************************************************************

size_t find_min(size_t * cnt, size_t buf_size)
{
    if (cnt == NULL || buf_size == 0)
    {
        perror("find_min: Invalid parameters\n");
        return buf_size + 1;
    }

    size_t i = 0;
    size_t min = 0;

    for (i = 1; i < buf_size; i++)
    {
        if(cnt[i] < cnt[min])
        {
            min = i;
        }
    }

    return min;
}

//****************************************************************************
//****************************************************************************

size_t get_next(FILE ** F, char ** buf, size_t * cnt, size_t * overest, size_t buf_size, size_t max_str_len)
{
    if (*F == NULL || buf == NULL || cnt == NULL || buf_size == 0 || max_str_len == 0)
    {
        perror("read_initial_buf: Invalid parameters\n");
        return 1;
    }

    char * str = NULL;
    size_t idx = 0;
    size_t inc_cnt = 0;

    str = (char*)malloc(sizeof(char)*max_str_len);

    if (!fgets(str, max_str_len, *F))
    {
        free(str);
        return 2;
    }
    remove_eol(str);
    idx = find_in_buf(buf, str, buf_size, max_str_len);

    if (idx > buf_size)
    {
        // Если строка не найдена
        idx = find_min(cnt, buf_size);
        inc_cnt = cnt[idx] + 1;
        swap_buffer_elements(&buf[idx], &str, &cnt[idx], &inc_cnt);
    }
    else
    {
        // Если строка найдена, то увеличить ее счетчик
        cnt[idx] += 1;
    }

    free(str);
    return 0;
}

//****************************************************************************
//****************************************************************************
