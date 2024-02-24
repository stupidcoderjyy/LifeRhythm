//
// Created by stupid_coder_jyy on 2024/2/22.
//

#ifndef LIFERHYTHM_SERIALIZABLE_H
#define LIFERHYTHM_SERIALIZABLE_H

#include <IByteReader.h>
#include <IByteWriter.h>

template<class T>
class Serializable {
public:
    virtual void toBytes(IByteWriter* writer) = 0;
    virtual void fromBytes(IByteReader* reader) = 0;
};

#endif //LIFERHYTHM_SERIALIZABLE_H
