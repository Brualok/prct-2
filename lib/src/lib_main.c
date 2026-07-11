/*
lib_main.c - главный модуль библиотеки. 

Терземан Андрей Александрович
МК-101
*/
#include "lib_main.h"
#include <stdlib.h>
#include <string.h>

#ifndef N
#endif

static int match_at(const unsigned char* data, size_t data_len,
    size_t pos, const unsigned char* pat, size_t pat_len)
{
    if (pos + pat_len > data_len) return 0;
    for (size_t i = 0; i < pat_len; ++i) {
        if (data[pos + i] != pat[i]) return 0;
    }
    return 1;
}

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
    if (pat_len == 0) {
        unsigned char buffer[N];
        size_t bytes;
        while ((bytes = fread(buffer, 1, N, in)) > 0)
            fwrite(buffer, 1, bytes, out);
        return 0;
    }