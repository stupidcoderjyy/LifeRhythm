//
// Created by stupid_coder_jyy on 2024/2/17.
//

#ifndef LIFERHYTHM_STRINGINPUT_H
#define LIFERHYTHM_STRINGINPUT_H

#include "AbstractInput.h"
#include <QVector>

class CORE_API StringInput : public AbstractInput{
private:
    QByteArray data;
    QVector<int> marks{};
    int next;
public:
    explicit StringInput(const QString& str);
    int read() override;
    int forward() override;
    void mark() override;
    void removeMark() override;
    void recover(bool consumeMark) override;
    void recover() override;
    QString capture() override;
    bool available() const override;
    int retract(int count) override;
    int retract() override;
private:
    QString capture(int end, int start) const;
};


#endif //LIFERHYTHM_STRINGINPUT_H
