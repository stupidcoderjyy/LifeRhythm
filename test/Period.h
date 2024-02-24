//
// Created by stupid_coder_jyy on 2024/2/24.
//

#ifndef LIFERHYTHM_PERIOD_H
#define LIFERHYTHM_PERIOD_H

#include "ElementsManager.h"
#include "RangeBar.h"

class Period : public Serializable, public RangeWidgetData {
protected:
    QString info;
public:
    Period();
    Period(int begin, int end, QString info);
    void toBytes(IByteWriter *writer) override;
    void fromBytes(IByteReader *reader) override;
    const QString &getInfo() const;
    void setInfo(const QString &info);
};

#endif //LIFERHYTHM_PERIOD_H
