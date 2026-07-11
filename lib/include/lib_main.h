/*
lib_main.h - заголовочный файл модуля lib_main.c. 

Терземан Андрей Александрович
МК-101
*/

#pragma once
#ifndef REPLACE_H
#define REPLACE_H

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

typedef struct {
    uint8_t* data;    // массив байт
    size_t length;    // количество байт
    size_t match_idx; // вспомогательный индекс (если используется)
    int error;        // 0 – успех, 1 – ошибка
} hex_to_bytes;

hex_to_bytes strtobyte(const char* input_str);

int process_file(FILE* in, FILE* out,
    const unsigned char* pattern, size_t pat_len,
    const unsigned char* replacement, size_t repl_len);

#endif