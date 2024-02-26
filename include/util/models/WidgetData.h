//
// Created by stupid_coder_jyy on 2024/2/25.
//

#ifndef LIFERHYTHM_WIDGET_DATA_H
#define LIFERHYTHM_WIDGET_DATA_H

#include <QObject>
#include "Serializable.h"

class WidgetData : public QObject, public Serializable{
    Q_OBJECT
public:
    WidgetData();
    template<class T> inline T* cast() {
        return static_cast<T*>(this);
    }
    void toBytes(IByteWriter *writer) override;
    void fromBytes(IByteReader *reader) override;
signals:
    void sigDataChanged();
};


#endif //LIFERHYTHM_WIDGET_DATA_H
