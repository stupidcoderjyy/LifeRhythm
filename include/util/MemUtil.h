//
// Created by stupid_coder_jyy on 2023/12/30.
//

#ifndef LIFERHYTHM_MEMUTIL_H
#define LIFERHYTHM_MEMUTIL_H

int** allocateArray(int rows, int columns);
int* allocateArray(int size);

void freeArray(int** p, int rows);

#define DELETE_MAP(qMap) \
    for (auto it = (qMap).begin() ; it != (qMap).end() ; it++) {\
        delete it.value();\
    }\
    (qMap).clear();
#define DELETE_MAP_P(qMap) \
    for (auto it = (qMap)->begin() ; it != (qMap)->end() ; it++) {\
        delete it.value();\
    }\
    (qMap)->clear();
#define DELETE_LIST_P(pList) \
    for (auto* d : *(pList)) {\
        delete d;\
    }\
    (pList)->clear();
#define DELETE_LIST(list) \
    for (auto* d : list) {\
        delete d;\
    }\
    list.clear();
#define DELETE_NESTED_LIST(list) \
    for (auto* child : list) {\
        DELETE_LIST_P(child);\
        delete child;\
    }\
    list.clear();
#endif //LIFERHYTHM_MEMUTIL_H
