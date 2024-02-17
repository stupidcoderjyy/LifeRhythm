//
// Created by stupid_coder_jyy on 2024/2/11.
//

#ifndef LIFERHYTHM_MAINFRAME_H
#define LIFERHYTHM_MAINFRAME_H

#include <QMainWindow>
#include "Namespaces.h"

LR_BEGIN

class TabBar;

class MainFrame : public QWidget {
    friend class LifeRhythm;
private:
    TabBar* tabBar;
public:
    MainFrame();
};

LR_END


#endif //LIFERHYTHM_MAINFRAME_H
