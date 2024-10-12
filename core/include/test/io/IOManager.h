//
// Created by JYY on 24-10-11.
//

#ifndef IOMANAGER_H
#define IOMANAGER_H

#include <QMap>
#include "Identifier.h"

class IByteWriter;
class IByteReader;

class ISerializable {
public:
    virtual ~ISerializable() = default;
    virtual void load(IByteReader* in) = 0;
    virtual void clear() = 0;
    virtual void save(IByteWriter* out) = 0;
};

class IOManager {
private:
    QMap<Identifier, ISerializable*> elements;
public:
    void registerSerializable(const Identifier& loc, ISerializable* serializable);
    void load();
    void clear();
    void save();
};

#endif //IOMANAGER_H
