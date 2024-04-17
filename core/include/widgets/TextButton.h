//
// Created by stupid_coder_jyy on 2024/3/12.
//

#ifndef LIFERHYTHM_TEXTBUTTON_H
#define LIFERHYTHM_TEXTBUTTON_H

#include "TextLabel.h"
#include "FocusContainer.h"

class TextButton : public FocusContainer {
    Q_OBJECT
public:
    enum Type {
        Highlighted,
        Normal,
        Disabled
    };
private:
    TextLabel* content;
    Type type;
public:
    explicit TextButton(QWidget* parent = nullptr);
    void setButtonType(Type t);
    void setButtonText(const QString& text);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
signals:
    void sigActivated();
protected:
    QString getNormalQss() override;
    QString getFocusedQss() override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif //LIFERHYTHM_TEXTBUTTON_H
