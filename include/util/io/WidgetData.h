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

class IListModel : public QObject {
    Q_OBJECT
public:
    WidgetData* operator[](int idx);
    void operator<<(WidgetData* d);
    virtual int length() const = 0;
    virtual WidgetData* at(int idx) noexcept = 0;
    virtual void append(WidgetData* data) = 0;
    virtual void insert(int idx, WidgetData* data) = 0;
    virtual WidgetData* remove(int idx) = 0;
signals:
    void sigDataChanged(int begin, int end);
};


#endif //LIFERHYTHM_WIDGET_DATA_H
