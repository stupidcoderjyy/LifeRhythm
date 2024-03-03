//
// Created by stupid_coder_jyy on 2024/3/2.
//

#ifndef LIFERHYTHM_TYPE_H
#define LIFERHYTHM_TYPE_H

#include "WidgetData.h"

class Type : public WidgetData {
private:
    QString color;
    QString name;
public:
    Type(QString color, QString name);
    Type(const Type& o);
};


#endif //LIFERHYTHM_TYPE_H
