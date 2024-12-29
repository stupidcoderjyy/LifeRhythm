//
// Created by stupid_coder_jyy on 2024/2/17.
//

#ifndef STRINGINPUT_H
#define STRINGINPUT_H

#include "AbstractInput.h"
#include <QVector>

BEGIN_LR

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

END_LR

#endif //STRINGINPUT_H
