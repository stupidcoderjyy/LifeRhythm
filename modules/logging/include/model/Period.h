//
// Created by stupid_coder_jyy on 2024/3/2.
//

#ifndef LIFERHYTHM_PERIOD_H
#define LIFERHYTHM_PERIOD_H

#include "ListData.h"
#include "PeriodType.h"
#include "DataLoader.h"
#include "Namespaces.h"

BEGIN_NAMESPACE(lr::log)

class Period : public WidgetData {
    Q_OBJECT
public:
    int begin;
    int end;
    QString info;
    Period(int begin, int end, QString info);
    Period();
    void toBytes(IByteWriter *writer) override;
    void fromBytes(IByteReader *reader) override;
};

class PeriodList : public ListData {
protected:
    WidgetData *readElement(IByteReader *reader) override;
};

class PeriodDataLoader : public ListDataLoader {
public:
    explicit PeriodDataLoader(QString destPath);
protected:
    WidgetData *createData() noexcept override;
};

END_NAMESPACE

#endif //LIFERHYTHM_PERIOD_H
