//
// Created by stupid_coder_jyy on 2023/12/28.
//

#include "Preconditions.h"
#include "Error.h"

void Preconditions::checkIndexRange(int min, int max, int actual, const QString &where) {
    if (actual < min || actual > max) {
        throw Error(where);
    }
}

void Preconditions::checkIndexRange(int min, int max, int actual, const QString &where, const QString &what) {
    if (actual < min || actual > max) {
        throw Error("Invalid Range", where, what);
    }
}

void Preconditions::checkNotNull(void *p, const QString &where, const QString &what) {
    if (!p) {
        throw Error("Null Pointer", where, what);
    }
}

void Preconditions::checkNotNull(void *p, const QString &where) {
    if (!p) {
        throw Error(where);
    }
}

void Preconditions::checkState(bool state, const QString &where, const QString &what) {
    if (!state) {
        throw Error("invalid state", where, what);
    }
}

void Preconditions::checkState(bool state, const QString &where) {
    if (!state) {
        throw Error(where);
    }
}

void Preconditions::checkArgument(bool state, const QString &where, const QString &what) {
    if (!state) {
        throw Error("invalid argument", where, what);
    }
}

void Preconditions::checkArgument(bool state, const QString &where) {
    if (!state) {
        throw Error(where);
    }
}
