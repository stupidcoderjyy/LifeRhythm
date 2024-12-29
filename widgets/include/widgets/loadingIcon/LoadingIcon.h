//
// Created by JYY on 24-12-13.
//

#ifndef LOADINGICON_H
#define LOADINGICON_H
#include <Namespaces.h>
#include <QTimer>
#include <Widget.h>

BEGIN_LR

class LoadingIcon : public Widget {
    Q_OBJECT
private:
    QTimer timer;
    int angle;
public:
    explicit LoadingIcon(QWidget* parent = nullptr, bool iic = true);
    void startLoading();
    void stopLoading();
protected:
    void paintEvent(QPaintEvent *event) override;
private slots:
    void updateRotation();
};

END_LR

#endif //LOADINGICON_H
