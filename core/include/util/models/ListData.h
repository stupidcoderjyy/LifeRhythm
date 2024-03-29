//
// Created by stupid_coder_jyy on 2024/2/29.
//

#ifndef LIFERHYTHM_LISTDATA_H
#define LIFERHYTHM_LISTDATA_H

#include "WidgetData.h"

class ListData : public WidgetData {
protected:
    int changeBegin, changeEnd;
    bool editing;
    QVector<WidgetData*> data;
public:
    ListData();
    void beginEdit();
    void endEdit();
    void markChange(int min, int max);
    virtual int length() const;
    virtual WidgetData* at(int idx) noexcept;
    virtual void append(WidgetData* data);
    virtual void insert(int idx, WidgetData* data);
    virtual WidgetData* remove(int idx);
    void toBytes(IByteWriter *writer) override;
    void fromBytes(IByteReader *reader) override;
    int getChangeBegin() const;
    int getChangeEnd() const;
    QVector<WidgetData*>& getData();
protected:
    virtual WidgetData* readElement(IByteReader* reader);
};


#endif //LIFERHYTHM_LISTDATA_H
