//
// Created by JYY on 24-12-24.
//

#ifndef STYLEGROUPSTORAGE_H
#define STYLEGROUPSTORAGE_H

#include "RcManager.h"
#include <QTextCharFormat>
#include "Namespaces.h"

BEGIN_LR

typedef QMap<QString, QTextCharFormat> StyleGroup;

class CORE_API StyleGroupStorage : public MultiSourceResourceManager<StyleGroup> {
    STATIC_DEFINE(StyleGroupStorage, StyleGroup)
};

END_LR

#endif //STYLEGROUPSTORAGE_H
