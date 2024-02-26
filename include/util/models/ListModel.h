//
// Created by stupid_coder_jyy on 2024/2/26.
//

#ifndef LIFERHYTHM_LISTMODEL_H
#define LIFERHYTHM_LISTMODEL_H

#include "WidgetData.h"
#include "DataLoader.h"

class IListModel : public QObject {
Q_OBJECT
public:
    inline void operator<<(WidgetData* d) {
        append(d);
    }
    virtual int length() const = 0;
    virtual WidgetData* at(int idx) noexcept = 0;
    virtual void append(WidgetData* data) = 0;
    virtual void insert(int idx, WidgetData* data) = 0;
    virtual WidgetData* remove(int idx) = 0;
signals:
    void sigDataChanged(int begin, int end);
};

class ListModel : public ListDataLoader<WidgetData>, public IListModel{
public:
    ListModel(const QString& path, const QString& fileName);
    int length() const override;
    WidgetData *at(int idx) noexcept override;
    void append(WidgetData *data) override;
    void insert(int idx, WidgetData *data) override;
    WidgetData* remove(int idx) override;
};

#endif //LIFERHYTHM_LISTMODEL_H
