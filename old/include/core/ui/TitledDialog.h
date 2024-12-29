//
// Created by stupid_coder_jyy on 2024/2/16.
//

#ifndef LIFERHYTHM_TITLEDDIALOG_H
#define LIFERHYTHM_TITLEDDIALOG_H

#include <QDialog>

#include "Button.h"
#include "Label.h"
#include "StandardWidget.h"
#include "Widget.h"
#include "Namespaces.h"
#include "Plugin.h"

BEGIN_NAMESPACE(lr)

class DialogCloseButton;

class CORE_API TitledDialog : public QDialog, public StandardWidget {
    friend class LifeRhythm;
private:
    DialogCloseButton* closeButton{};
    Label* titleLabel{};
public:
    explicit TitledDialog(QWidget* parent, bool initInConstructor = true);
    void setContent(const QString& title, QWidget* widget);
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
};

class CORE_API DialogCloseButton : public Button {
public:
    explicit DialogCloseButton(QWidget* parent);
protected:
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
};

class CORE_API TopWidget : public Widget {
private:
    bool drag{};
    QPoint mouseStartPos{};
    QPoint windowPos{};
public:
    explicit TopWidget(QWidget* parent);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

END_NAMESPACE

#endif //LIFERHYTHM_TITLEDDIALOG_H
