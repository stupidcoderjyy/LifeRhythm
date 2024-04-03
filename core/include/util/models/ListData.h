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
    inline int length() const;
    inline WidgetData* at(int idx) noexcept;
    virtual void append(WidgetData* data);
    virtual void insert(int idx, WidgetData* data);
    virtual WidgetData* remove(int idx);
    void toBytes(IByteWriter *writer) override;
    void fromBytes(IByteReader *reader) override;
    inline int getChangeBegin() const;
    inline int getChangeEnd() const;
    inline QVector<WidgetData*>& getData();
protected:
    virtual WidgetData* readElement(IByteReader* reader);
};

inline int ListData::length() const {
    return data.length();
}

inline WidgetData *ListData::at(int idx) noexcept {
    if (idx < 0 || idx >= data.length()) {
        return nullptr;
    }
    return data.at(idx);
}

inline int ListData::getChangeBegin() const {
    return changeBegin;
}

inline int ListData::getChangeEnd() const {
    return changeEnd;
}

inline QVector<WidgetData *> &ListData::getData() {
    return data;
}


#endif //LIFERHYTHM_LISTDATA_H
