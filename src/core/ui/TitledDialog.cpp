//
// Created by stupid_coder_jyy on 2024/2/16.
//

#include "TitledDialog.h"
#include "WidgetUtil.h"
#include "Widget.h"
#include "RcManagers.h"
#include <QTimer>
#include <QPropertyAnimation>
#include <QLayout>
#include <QMouseEvent>

USING_NAMESPACE(lr)

TitledDialog::TitledDialog(QWidget *parent, bool initInConstructor):QDialog(parent), StandardWidget(initInConstructor) {
}

void TitledDialog::onFinishedParsing(Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        auto* dialog = static_cast<TitledDialog*>(target);
        dialog->closeButton = dialog->getPointer<DialogCloseButton>("closeButton");
        dialog->titleLabel = dialog->getPointer<Label>("titleLabel");
        dialog->setAttribute(Qt::WA_DeleteOnClose, true);
        dialog->setWindowFlags(Qt::FramelessWindowHint);
        dialog->setWindowModality(Qt::WindowModality::WindowModal);
        connect(dialog->closeButton, &DialogCloseButton::sigActivated, dialog, [dialog](){
            auto* closeAnimation = new QPropertyAnimation(dialog, "windowOpacity");
            closeAnimation->setDuration(100);
            closeAnimation->setStartValue(1);
            closeAnimation->setEndValue(0);
            closeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            QTimer::singleShot(100,dialog,&QDialog::close);
        });
    };
}

void TitledDialog::setContent(const QString &title, QWidget *widget) {
    titleLabel->setText(title);
    widget->setParent(this);
    layout()->addWidget(widget);
}

DialogCloseButton::DialogCloseButton(QWidget *parent): Button(parent) {
}

void DialogCloseButton::enterEvent(QEvent *event) {
    setState(1);
}

void DialogCloseButton::leaveEvent(QEvent *event) {
    setState(0);
}

void DialogCloseButton::mouseReleaseEvent(QMouseEvent *ev) {
    if (isHovered(this, ev)) {
        emit sigActivated();
    }
}

TopWidget::TopWidget(QWidget *parent): Widget(parent) {
}

void TopWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        drag = true;
        mouseStartPos = event->globalPos();
        windowPos = static_cast<QWidget*>(parent())->frameGeometry().topLeft();
    }
}

void TopWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        drag = false;
    }
}

void TopWidget::mouseMoveEvent(QMouseEvent *event) {
    if (drag) {
        static_cast<QWidget*>(parent())->move(windowPos + event->globalPos() - mouseStartPos);
    }
}