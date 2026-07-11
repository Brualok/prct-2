/*
lib_main.c -  Реализация функций библиотеки замены байтовых последовательностей.
         Содержит статические вспомогательные функции для поиска и основную функцию process_file.

Терземан Андрей Александрович
МК-101
*/
#include "lib_main.h"
#include <stdlib.h>
#include <string.h>

#ifndef N
#endif

//match_at – проверяет, совпадает ли паттерн с данными в буфере на позиции pos.
static int match_at(const unsigned char* data, size_t data_len,
    size_t pos, const unsigned char* pat, size_t pat_len)
{
    if (pos + pat_len > data_len) return 0;
    for (size_t i = 0; i < pat_len; ++i) {
        if (data[pos + i] != pat[i]) return 0;
    }
    return 1;
}


/* find_match – ищет первое вхождение паттерна в буфере в диапазоне позиций
  от start до end включительно. Использует match_at для проверки каждой позиции.*/

static size_t find_match(const unsigned char* data, size_t data_len,
    size_t start, size_t end,
    const unsigned char* pat, size_t pat_len)
{
    for (size_t pos = start; pos <= end; ++pos) {
        if (match_at(data, data_len, pos, pat, pat_len))
            return pos;
    }
    return (size_t)-1;
}


int process_file(FILE* in, FILE* out,
    const unsigned char* pat, size_t pat_len,
    const unsigned char* repl, size_t repl_len)
{ 
    // Случай пустого паттерна (длина 0) – просто копируем файл без замен.
    if (pat_len == 0) {
        unsigned char buffer[N];
        size_t bytes;
        while ((bytes = fread(buffer, 1, N, in)) > 0)
            fwrite(buffer, 1, bytes, out);
        return 0;
    }

    
    /*Выделяем динамический буфер размером N + pat_len.
     * Максимальный размер при pat_len ≤ 2N составляет 3N, что ≤ 4N.*/
    size_t buf_size = N + pat_len;
    unsigned char* buf = (unsigned char*)malloc(buf_size);
    if (!buf) return -1;

    size_t head = 0;       // смещение начала данных в буфере
    size_t len = 0;        // общая длина данных в буфере
    size_t tail_len = pat_len - 1; // количество байт, которые нужно сохранять как хвост

    // Читаем первый блок (размер N) в начало буфера
    size_t bytes_read = fread(buf + head, 1, N, in);
    if (bytes_read == 0) {
        free(buf);
        return 0; 
    }
    len = bytes_read;

    while (1) {
        size_t safe_end = (len > tail_len) ? (len - tail_len) : 0;
        size_t pos = 0;

        while (pos < safe_end) {
            size_t match = find_match(buf + head, len, pos, safe_end - pat_len, pat, pat_len);
            if (match != (size_t)-1)
            {
                fwrite(buf + head + pos, 1, match - pos, out);
                fwrite(repl, 1, repl_len, out);
                pos = match + pat_len;
            }
            else {
               
                fwrite(buf + head + pos, 1, safe_end - pos, out);
                pos = safe_end;
                break;
            }
        }

        
        if (len > tail_len) 
        {
            head += (len - tail_len);
            len = tail_len;
        }
        

        
        size_t free_space = buf_size - (head + len);
        if (free_space < N) 
        {
            free(buf);
            return -1;
        }
        bytes_read = fread(buf + head + len, 1, N, in);
        if (bytes_read == 0) {
            if (len > 0)
                fwrite(buf + head, 1, len, out);
            break;
        }
        len += bytes_read;
    }

    free(buf);
    return 0;
}