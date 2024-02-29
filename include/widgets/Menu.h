//
// Created by stupid_coder_jyy on 2024/2/28.
//

#ifndef LIFERHYTHM_MENU_H
#define LIFERHYTHM_MENU_H

#include <QMenu>
#include "StandardWidget.h"
#include "Styles.h"

class Menu : public QWidget, public StandardWidget {
    Q_OBJECT
public:
    explicit Menu(QWidget* parent = nullptr);
signals:
    void sigAboutToClose();
protected:
    void focusOutEvent(QFocusEvent *event) override;
    void showEvent(QShowEvent *event) override;
PAINT_QSS
};


#endif //LIFERHYTHM_MENU_H
