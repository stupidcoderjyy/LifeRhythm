//
// Created by stupid_coder_jyy on 2024/2/28.
//

#ifndef ARROWBUTTON_H
#define ARROWBUTTON_H

#include "Button.h"
#include "Widget.h"

BEGIN_LR

class ArrowButton : public Button {
    Q_OBJECT
public:
    enum Type {
        Right,
        Down,
        Left,
        Up,
        None
    };
private:
    Type baseType;
    Type activatedType;
    Type showType;
public:
    explicit ArrowButton(QWidget* parent = nullptr, bool initInConstructor = true);
    void setActivatedType(Type t);
    void setBaseType(Type t);
    void setSelected(bool selected) override;
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    void initWidget() override;
private:
    void updateBase();
    static Type fromString(const QString &str);
};

END_LR

#endif //LIFERHYTHM_ARROWBUTTON_H
