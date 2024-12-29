//
// Created by stupid_coder_jyy on 2024/2/28.
//

#ifndef MENU_H
#define MENU_H

#include <QMenu>
#include "StandardWidget.h"
#include "Styles.h"
#include <QPainter>

BEGIN_LR

class Menu : public QWidget, public StandardWidget {
    Q_OBJECT
public:
    explicit Menu(QWidget* parent = nullptr, bool initInConstructor = true);
signals:
    void sigAboutToClose();
protected:
    void focusOutEvent(QFocusEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void initWidget() override;
PAINT_QSS
};

END_LR

#endif //MENU_H
