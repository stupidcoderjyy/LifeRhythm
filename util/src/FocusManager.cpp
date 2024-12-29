//
// Created by stupid_coder_jyy on 2024/3/13.
//

#include "FocusManager.h"
#include <QDebug>

USING_LR

FocusManager* FocusManager::manager = new FocusManager();

void FocusManager::mark(QWidget *w) {
    manager->mark0(w);
}

void FocusManager::push(QWidget* w) {
    manager->push0(w);
}

void FocusManager::pop() {
    manager->pop0();
}

bool FocusManager::checkPeek(QWidget *w) {
    return manager->checkPeek0(w);
}

void FocusManager::popIfPeekMatch(QWidget *w) {
    if (manager->checkPeek0(w)) {
        manager->pop0();
    }
}

bool FocusManager::checkCurrent(QWidget *w) {
    return manager->focusedWidget == w;
}

FocusManager::FocusManager(): focusedWidget() {
}

void FocusManager::mark0(QWidget *w) {
    if (focusedWidget == w) {
        return;
    }
    if (focusedWidget) {
        focusedWidget->clearFocus();
    }
    focusedWidget = w;
    if (w) {
        w->setFocus();
    }
}

void FocusManager::push0(QWidget *w) {
    if (!focusStack.empty() && focusStack.last() == w) {
        return;
    }
    focusStack << w;
}

void FocusManager::pop0() {
    if (!focusStack.empty()) {
        mark0(focusStack.takeLast());
    }
}

bool FocusManager::checkPeek0(const QWidget *w) {
    return !focusStack.empty() && focusStack.last() == w;
}
