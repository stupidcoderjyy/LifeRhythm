//
// Created by stupid_coder_jyy on 2024/3/30.
//

#ifndef LIFERHYTHM_ICONTEXTBUTTON_H
#define LIFERHYTHM_ICONTEXTBUTTON_H

#include "FocusContainer.h"
#include "Identifier.h"

class ImgLabel;
class TextLabel;

class CORE_API IconTextButton : public FocusContainer {
    Q_OBJECT
private:
    bool hasInit;
    ImgLabel* icon;
    TextLabel* name;
public:
    explicit IconTextButton(QWidget* parent = nullptr);
    void set(const Identifier& iconLoc, const QString& name);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
signals:
    void sigActivated();
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
private:
    void init();
    void setText(const QString& text);
};


#endif //LIFERHYTHM_ICONTEXTBUTTON_H
