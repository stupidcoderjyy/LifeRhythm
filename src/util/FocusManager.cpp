//
// Created by stupid_coder_jyy on 2024/3/13.
//

#include "FocusManager.h"

FocusManager* FocusManager::manager = new FocusManager();

void FocusManager::markFocus(QWidget *w) {
    if (manager->focusedWidget == w) {
        return;
    }
    if (manager->focusedWidget) {
        manager->focusedWidget->clearFocus();
    }
    manager->focusedWidget = w;
    if (w) {
        w->setFocus();
    }
}

FocusManager::FocusManager(): focusedWidget() {
}
