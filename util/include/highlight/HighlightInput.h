//
// Created by JYY on 25-1-3.
//

#ifndef HIGHLIGHTINPUT_H
#define HIGHLIGHTINPUT_H
#include "Namespaces.h"
#include "StringInput.h"

BEGIN_LR

class HighlightInput : public StringInput {
private:
    int p;
public:
    explicit HighlightInput(const QString &blockText);
    int read() override;
    QString readUtf() override;
    void retractUtf(int count);
    int retract() override;
    int retract(int count) override;
    int approachUtf(int ch);
    int approachUtf(int ch1, int ch2);
    int approachUtf(int ch1, int ch2, int ch3);
    int approachUtf(const std::initializer_list<int> &list);
    inline int pos() const;
};

inline int HighlightInput::pos() const {
    return p;
}

END_NP

#endif //HIGHLIGHTINPUT_H
