#pragma once
#include <cstdio>

void fopen_file(FILE* fptr, const char* pathname, const char* mode) {
    fopen_s(&fptr, pathname, mode);
}

