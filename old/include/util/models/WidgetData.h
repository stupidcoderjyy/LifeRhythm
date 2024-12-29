//
// Created by stupid_coder_jyy on 2024/2/25.
//

#ifndef LIFERHYTHM_WIDGET_DATA_H
#define LIFERHYTHM_WIDGET_DATA_H

#include <QObject>
#include "Plugin.h"

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

#endif //LIFERHYTHM_WIDGET_DATA_H
