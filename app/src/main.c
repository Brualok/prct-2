/*
main.c - главный модуль программы. 

Терземан Андрей Александрович
МК-101
*/
#include <stdio.h>
#include <stdlib.h>
#include "lib_main.h"


int main(int argc, char* argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Error: exactly 4 arguments required: %s <infile> <outfile> <pattern_hex> <replacement_hex>\n", argv[0]);
        return 1;
    }
    char* infile = argv[1];
    char* outfile = argv[2];
    char* pat_hex = argv[3];
    char* rep_hex = argv[4];

    return 0;
}