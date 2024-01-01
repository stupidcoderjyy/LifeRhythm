//
// Created by stupid_coder_jyy on 2023/12/30.
//

#ifndef LIFERHYTHM_MEMUTIL_H
#define LIFERHYTHM_MEMUTIL_H

int** allocateArray(int rows, int columns);
int* allocateArray(int size);

void freeArray(int** p, int rows);

size_t allocatedMem();
#endif //LIFERHYTHM_MEMUTIL_H
