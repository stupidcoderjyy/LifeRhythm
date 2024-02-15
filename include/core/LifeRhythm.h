//
// Created by stupid_coder_jyy on 2024/2/9.
//

#ifndef LIFERHYTHM_LIFERHYTHM_H
#define LIFERHYTHM_LIFERHYTHM_H

#include <QString>
#include <QObject>

class MainFrame;

class LifeRhythm : public QObject{
    Q_OBJECT
private:
    static LifeRhythm* lr;
    MainFrame* mainFrame;
public:
    static LifeRhythm* get();
    static void launch();
signals:
    void sigPreInit();
    void sigMainInit();
    void sigPostInit();
private:
    LifeRhythm();
    void launch0();
    void preInit();
    void mainInit();
    void postInit();
};


#endif //LIFERHYTHM_LIFERHYTHM_H
