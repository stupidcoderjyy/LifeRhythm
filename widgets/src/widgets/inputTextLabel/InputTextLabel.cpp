//
// Created by JYY on 24-11-7.
//

#include "InputTextLabel.h"

#include <Helpers.h>
#include <qboxlayout.h>
#include <Styles.h>

USING_LR

InputTextLabel::InputTextLabel(QWidget *parent): Label(parent), edit(new LineEdit()), editing() {
    setFixedHeight(QFontMetrics(font()).height() + 5);
    edit->hide();
    edit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    connect(edit, &LineEdit::editingFinished, this, [this] {
        stopEditing();
    });
}

void InputTextLabel::initEditor() {
    edit->setText(text());
}

void InputTextLabel::finishEditing() {
    setText(edit->text());
}

void InputTextLabel::mouseDoubleClickEvent(QMouseEvent *event) {
    edit->setParent(parentWidget());
    edit->setGeometry(geometry());
    edit->show();
    initEditor();
    setText("");
    edit->setFocus();
    edit->selectAll();
    editing = true;
}

void InputTextLabel::resizeEvent(QResizeEvent *event) {
    stopEditing();
}

void InputTextLabel::stopEditing() {
    if (editing) {
        finishEditing();
        edit->hide();
        show();
        editing = false;
    }
}
