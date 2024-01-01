//
// Created by stupid_coder_jyy on 2023/12/31.
//

#include <cstring>
#include "MemUtil.h"
#include <windows.h>
#include <psapi.h>

int **allocateArray(int rows, int columns) {
    int** p = new int*[rows];
    for (int i = 0 ; i < rows ; i ++) {
        p[i] = new int[columns];
        memset(p[i], 0, columns << 2);
    }
    return p;
}

int *allocateArray(int size) {
    int* p = new int[size];
    memset(p, 0, size << 2);
    return p;
}

void freeArray(int **p, int rows) {
    for (int i = 0 ; i < rows ; i ++) {
        delete[] p[i];
    }
    delete[] p;
}

size_t allocatedMem() {
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(),&pmc,sizeof(pmc));
    return pmc.WorkingSetSize;
}
