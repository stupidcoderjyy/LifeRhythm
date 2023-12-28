//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef LIFERHYTHM_PRECONDITIONS_H
#define LIFERHYTHM_PRECONDITIONS_H

#include <QString>

class Preconditions {
public:
    static void checkIndexRange(int min, int max, int actual, const QString& where);
    static void checkIndexRange(int min, int max, int actual, const QString& where, const QString& what);
    static void checkNotNull(void* p, const QString& where);
    static void checkNotNull(void* p, const QString& where, const QString& what);
    static void checkState(bool state, const QString& where);
    static void checkState(bool state, const QString& where, const QString& what);
    static void checkArgument(bool state, const QString& where);
    static void checkArgument(bool state, const QString& where, const QString& what);
};
#endif //LIFERHYTHM_PRECONDITIONS_H
