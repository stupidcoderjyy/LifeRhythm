//
// Created by stupid_coder_jyy on 2024/3/30.
//

#ifndef LIFERHYTHM_MODULE_H
#define LIFERHYTHM_MODULE_H

#include "Namespaces.h"
#include <QObject>

BEGIN_NAMESPACE(lr)

class Module : public QObject {
    friend class LifeRhythm;
    QString id;
public:
    explicit Module(QString name);
    const QString& getName();
protected:
    virtual void preInit();
    virtual void mainInit();
    virtual void postInit();
};

END_NAMESPACE

#endif //LIFERHYTHM_MODULE_H
