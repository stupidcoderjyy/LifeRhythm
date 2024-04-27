//
// Created by stupid_coder_jyy on 2024/3/2.
//

#ifndef LIFERHYTHM_USAGETYPE_H
#define LIFERHYTHM_USAGETYPE_H

#include <QColor>
#include "SelectableTreeData.h"
#include "DataLoader.h"
#include "Namespaces.h"

BEGIN_NAMESPACE(lr::log)

class UsageType : public WidgetData {
private:
    QColor color;
    QString name;
public:
    UsageType(QColor color, QString name);
    UsageType(const UsageType& o);
    UsageType();
    inline const QColor& getColor();
    inline const QString& getName();
};

inline const QColor &UsageType::getColor() {
    return color;
}

inline const QString &UsageType::getName() {
    return name;
}

END_NAMESPACE

#endif //LIFERHYTHM_USAGETYPE_H
