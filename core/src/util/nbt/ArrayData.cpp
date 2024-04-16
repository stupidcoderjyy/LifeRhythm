//
// Created by stupid_coder_jyy on 2023/12/28.
//

#include "NBT.h"
#include "ArrayData.h"
#include "Error.h"
#include <QStringBuilder>

ArrayData::ArrayData(): Data(ARR) {

}

void ArrayData::serialize(IByteWriter *writer) {
    writer->writeByte(ARR);
    writer->writeInt(data.size());
    for (const auto &item: data) {
        item->serialize(writer);
    }
}

void ArrayData::deserialize(IByteReader *reader) {
    int len = reader->readInt();
    data.reserve(len);
    for (int i = 0 ; i < len ; i ++) {
        data.append(parseData(reader));
    }
}

QString ArrayData::toString() {
    QString res = "[";
    int i = 0;
    while (i < data.size()) {
        res = res % data[i]->toString();
        if (++i == data.size()) {
            break;
        }
        res = res % ',';
    }
    return res % ']';
}

Data *ArrayData::copy() {
    return nullptr;
}

ArrayData::~ArrayData() {
    for (const auto &item: data) {
        delete item;
    }
}

ArrayData *ArrayData::add(int val) {
    data.append((new IntData())->setVal(val));
    return this;
}

ArrayData *ArrayData::add(ArrayData *val) {
    if (!val) {
        throwInFunc("null value");
    }
    data.append(val);
    return this;
}

ArrayData *ArrayData::add(const QString& val) {
    data.append((new StringData())->setVal(val));
    return this;
}

ArrayData *ArrayData::add(float val) {
    data.append((new FloatData())->setVal(val));
    return this;
}

ArrayData *ArrayData::add(NBT *val) {
    if (!val) {
        throwInFunc("null value");
    }
    data.append(val);
    return this;
}

void ArrayData::fillString(QStringList &target) {
    for (auto& e : data) {
        if (e->type != STRING) {
            continue;
        }
        target << e->asString()->get();
    }
}

void ArrayData::fillString(QStringList &target, int len, const QString &defaultVal) {
    int i = 0;
    target.reserve(qMax(len, data.length()));
    while (i < data.length()) {
        auto* e = data[i++];
        if (e->type == STRING) {
            target << e->asString()->get();
        }
    }
    while (i++ < len) {
        target << defaultVal;
    }
}

void ArrayData::fillInt(int *arr, int len) {
    for (auto& e : data) {
        if (len == 0) {
            break;
        }
        if (e->type == INT) {
            *arr++ = e->asInt()->get();
            len--;
        }
    }
}

void ArrayData::fillInt(QVector<int> &target, int len, int defaultVal) {
    int i = 0;
    int dl = data.length();
    target.reserve(qMax(len, dl));
    while (i < dl) {
        auto* e = data[i++];
        if (e->type == INT) {
            target << e->asInt()->get();
        }
    }
    while (i++ < len) {
        target << defaultVal;
    }
}

void ArrayData::fillFloat(float* arr, int len) {
    for (auto& e : data) {
        if (len == 0) {
            break;
        }
        if (e->type == Data::FLOAT) {
            *arr++ = e->asFloat()->get();
        } else if (e->type == Data::INT) {
            *arr++ = (float)e->asInt()->get();
        } else {
            continue;
        }
        len--;
    }
}

void ArrayData::fillBool(bool *arr, int len) {
    for (auto& e : data) {
        if (len == 0) {
            break;
        }
        if (e->type == BOOL) {
            *arr++ = e->asBool()->get();
            len--;
        }
    }
}
