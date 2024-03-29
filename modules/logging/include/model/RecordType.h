//
// Created by stupid_coder_jyy on 2024/3/5.
//

#ifndef LIFERHYTHM_RECORDTYPE_H
#define LIFERHYTHM_RECORDTYPE_H

#include "WidgetData.h"

class RecordType : public WidgetData {
public:
    QString iconPath;
    QString name;
public:
    RecordType(QString iconPath, QString name);
};


#endif //LIFERHYTHM_RECORDTYPE_H
