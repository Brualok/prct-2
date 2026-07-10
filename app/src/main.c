/*
main.c - главный модуль программы. 

Терземан Андрей Александрович
МК-101
*/
#include <stdio.h>
#include <stdlib.h>
#include "lib_main.h"


hex_to_bytes strtobyte(const char* input_str) {
    hex_to_bytes r = { NULL, 0, 0, 1 };
    if (input_str == NULL) {
        return r;
    }
    size_t len = strlen(input_str);
    if (len % 2 != 0) {
        return r;
    }
    size_t byte_len = len / 2;
    uint8_t* bytes = (uint8_t*)malloc(byte_len);
    if (!bytes) {
        return r;
    }
    for (size_t i = 0; i < byte_len; ++i) {
        unsigned int val;
        if (sscanf(input_str + 2 * i, "%02x", &val) != 1) {
            free(bytes);
            return r;
        }
        bytes[i] = (uint8_t)val;
    }
    r.data = bytes;
    r.length = byte_len;
    r.match_idx = 0;
    r.error = 0;
    return r;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Error: exactly 4 arguments required: %s <infile> <outfile> <pattern_hex> <replacement_hex>\n", argv[0]);
        return 1;
    }
    char* infile = argv[1];
    char* outfile = argv[2];
    char* pat_hex = argv[3];
    char* rep_hex = argv[4];

    FILE* input = fopen(infile, "rb");
    if (infile == NULL)
        printf("Error of open input file");
        return 1;
    FILE* output = fopen(outfile, "wb");
    if (outfile == NULL)
        printf("Error of open output file");
        return 1;
                
            

    return 0;
}