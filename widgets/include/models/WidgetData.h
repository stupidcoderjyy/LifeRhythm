//
// Created by stupid_coder_jyy on 2024/2/25.
//

#ifndef WIDGET_DATA_H
#define WIDGET_DATA_H

#include <QObject>
#include "Namespaces.h"
#include "Plugin.h"

namespace lr {

class CORE_API WidgetData : public QObject {
    Q_OBJECT
public:
    WidgetData() = default;
    template<class T> constexpr T* cast() {
        return static_cast<T*>(this);
    }
signals:
    void sigDataChanged();
};

END_LR

#endif //WIDGET_DATA_H
