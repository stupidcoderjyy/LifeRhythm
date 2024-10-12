//
// Created by stupid_coder_jyy on 2024/4/24.
//

#ifndef LIFERHYTHM_USAGE_H
#define LIFERHYTHM_USAGE_H

#include "Namespaces.h"
#include "UsageType.h"

BEGIN_NAMESPACE(lr::log)

class Usage : public WidgetData {
private:
    UsageType* type;
    QString name;
    int duration;
public:
    Usage(UsageType *type, QString name, int duration);
    Usage();
    inline UsageType *getType() const;
    inline const QString &getName() const;
    inline int getDuration() const;
};

inline UsageType *Usage::getType() const {
    return type;
}

inline const QString &Usage::getName() const {
    return name;
}

inline int Usage::getDuration() const {
    return duration;
}

END_NAMESPACE

#endif //LIFERHYTHM_USAGE_H
