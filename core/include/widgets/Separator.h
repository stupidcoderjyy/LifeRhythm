//
// Created by stupid_coder_jyy on 2024/4/20.
//

#ifndef LIFERHYTHM_SEPARATOR_H
#define LIFERHYTHM_SEPARATOR_H

#include "Widget.h"

class Separator : public Widget {
private:
    QColor color;
public:
    explicit Separator(QWidget* parent = nullptr);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    inline void setColor(const QColor& c);
protected:
    void paintEvent(QPaintEvent *event) override;
};

inline void Separator::setColor(const QColor &c) {
    color = c;
}

#endif //LIFERHYTHM_SEPARATOR_H
