//
// Created by stupid_coder_jyy on 2024/4/27.
//

#ifndef LIFERHYTHM_DATAA1_H
#define LIFERHYTHM_DATAA1_H

#include "Namespaces.h"
#include "WidgetData.h"
#include "Usage.h"

BEGIN_NAMESPACE(lr::log)

class DataA1 : public WidgetData {
private:
    Usage* usage;
    bool active;
public:
    DataA1(Usage* usage, bool active);
    DataA1();
    inline Usage* getUsage();
    inline int getDuration();
    inline bool isActive() const;
    inline bool isValid() const;
    void setUsage(Usage* usage);
    void setActive(bool active);
};

inline Usage* DataA1::getUsage() {
    return usage;
}

inline int DataA1::getDuration() {
    return usage->getDuration();
}

inline bool DataA1::isActive() const {
    return active;
}

inline bool DataA1::isValid() const {
    return usage;
}

END_NAMESPACE

#endif //LIFERHYTHM_DATAA1_H
