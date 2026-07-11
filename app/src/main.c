/*
main.c - главный модуль программы. 

Терземан Андрей Александрович
МК-101
*/
#include <stdio.h>
#include <string.h>
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
    if (input == NULL) 
    {
        fprintf(stderr, "Error: cannot open input file '%s'\n", infile);
        return 1;
    }
    FILE* output = fopen(outfile, "wb");
    if (output == NULL) 
    {
        fprintf(stderr, "Error: cannot open output file '%s'\n", outfile);
        return 1;
    }
    hex_to_bytes pat = strtobyte(pat_hex);                            
    if (pat.error) 
    {
        fprintf(stderr, "Error: invalid hex pattern string\n");
        fclose(input);
        fclose(output);
        return 1;
    }
    hex_to_bytes rep = strtobyte(rep_hex);
    if (rep.error) {
        fprintf(stderr, "Error: invalid hex replacement string\n");
        free(pat.data);
        fclose(input);
        fclose(output);
        return 1;
    }
    
    int result = process_file(input, output, pat.data, pat.length, rep.data, rep.length);

    
    free(pat.data);
    free(rep.data);

    
    fclose(input);  
    fclose(output);

    if (result != 0) {
        fprintf(stderr, "Error during file processing\n");
        return 1;
    }


    return 0;
}   