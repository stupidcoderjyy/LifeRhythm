//
// Created by stupid_coder_jyy on 2024/3/13.
//

#ifndef LIFERHYTHM_FOCUSMANAGER_H
#define LIFERHYTHM_FOCUSMANAGER_H

#include <QWidget>

class FocusManager {
private:
    static FocusManager* manager;
    QWidget* focusedWidget;
public:
    static void markFocus(QWidget* w);
protected:
    FocusManager();
};


#endif //LIFERHYTHM_FOCUSMANAGER_H
