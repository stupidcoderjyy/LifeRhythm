//
// Created by JYY on 24-12-24.
//

#ifndef STYLEGROUPRCLOADER_H
#define STYLEGROUPRCLOADER_H

#include <QTextCharFormat>
#include "RcLoader.h"

BEGIN_LR

class CORE_API StyleGroupRcLoader final : public RcLoader<QMap<QString, QTextCharFormat>> {
public:
    QMap<QString, QTextCharFormat> *load(const Identifier &loc, const QString &absolutePath) override;
SINGLETON_HEADER(StyleGroupRcLoader)
};

END_LR

#endif //STYLEGROUPRCLOADER_H
