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
#error "N must be defined at compile time (e.g., -DN=256)"
#endif
#define BUF_SIZE (2 * N)

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

static int match_at_dual(const unsigned char* prev, size_t prev_off, size_t prev_len,
    const unsigned char* cur, size_t cur_len,
    size_t pos, const unsigned char* pat, size_t pat_len)
{
    size_t total = prev_len + cur_len;
    if (pos + pat_len > total) return 0;
    for (size_t i = 0; i < pat_len; ++i) {
        size_t idx = pos + i;
        unsigned char byte;
        if (idx < prev_len) {
            byte = prev[prev_off + idx];
        }
        else {
            byte = cur[idx - prev_len];
        }
        if (byte != pat[i]) return 0;
    }
    return 1;
}

static size_t find_match_dual(const unsigned char* prev, size_t prev_off, size_t prev_len,
    const unsigned char* cur, size_t cur_len,
    size_t start, size_t search_end,
    const unsigned char* pat, size_t pat_len)
{
    size_t total = prev_len + cur_len;
    if (search_end >= total) search_end = total - pat_len;
    for (size_t pos = start; pos <= search_end; ++pos) {
        if (match_at_dual(prev, prev_off, prev_len, cur, cur_len, pos, pat, pat_len))
            return pos;
    }
    return (size_t)-1;
}
static void write_range_dual(FILE* out,
    const unsigned char* prev, size_t prev_off, size_t prev_len,
    const unsigned char* cur, size_t cur_len,
    size_t start, size_t end)
{
    size_t total = prev_len + cur_len;
    if (start >= end || start >= total) return;
    if (end > total) end = total;

    if (start < prev_len) {
        size_t len1 = (end < prev_len) ? (end - start) : (prev_len - start);
        fwrite(prev + prev_off + start, 1, len1, out);
        start = prev_len;
    }
    if (start < total) {
        size_t len2 = end - start;
        fwrite(cur + (start - prev_len), 1, len2, out);
    }
}
static void process_union(FILE* out,
    const unsigned char* prev_buf, size_t prev_off, size_t prev_len,
    const unsigned char* cur_buf, size_t cur_len,
    const unsigned char* pat, size_t pat_len,
    const unsigned char* repl, size_t repl_len,
    size_t* new_prev_len, size_t* new_prev_off)
{
    size_t total = prev_len + cur_len;
    size_t pos = 0;

    if (pat_len == 0 || total < pat_len) {
        *new_prev_len = cur_len;
        *new_prev_off = 0;
        return;
    }

    size_t tail_len = pat_len - 1;
    size_t safe_limit = total - tail_len;

    while (pos + pat_len <= total && pos < safe_limit) {
        size_t search_end = total - pat_len;
        if (search_end >= safe_limit) search_end = safe_limit - 1;

        size_t match = find_match_dual(prev_buf, prev_off, prev_len,
            cur_buf, cur_len,
            pos, search_end, pat, pat_len);

        if (match != (size_t)-1) {
            write_range_dual(out, prev_buf, prev_off, prev_len, cur_buf, cur_len, pos, match);
            fwrite(repl, 1, repl_len, out);
            pos = match + pat_len;
        }
        else {
            size_t output_end = total - pat_len;
            if (output_end > pos)
                write_range_dual(out, prev_buf, prev_off, prev_len, cur_buf, cur_len, pos, output_end);
            if (cur_len >= tail_len) {
                *new_prev_len = tail_len;
                *new_prev_off = cur_len - tail_len;
            }
            else {
                *new_prev_len = cur_len;
                *new_prev_off = 0;
            }
            return;
        }
    }

    size_t output_end = total - tail_len;
    if (output_end > pos)
        write_range_dual(out, prev_buf, prev_off, prev_len, cur_buf, cur_len, pos, output_end);

    if (cur_len >= tail_len) {
        *new_prev_len = tail_len;
        *new_prev_off = cur_len - tail_len;
    }
    else {
        *new_prev_len = cur_len;
        *new_prev_off = 0;
    }
}