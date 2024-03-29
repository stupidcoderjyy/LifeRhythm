//
// Created by stupid_coder_jyy on 2024/1/16.
//

#ifndef LIFERHYTHM_FACTORYINIT_H
#define LIFERHYTHM_FACTORYINIT_H

#include <QString>
#include "WidgetFactory.h"

class FactoryInit {
    friend class WidgetFactory;
private:
    static void mainInit(QMap<QString, WidgetFactory::Supplier>* stdSuppliers,
                         QMap<QString, StandardWidget*>* stdEmptyInstances);
};


#endif //LIFERHYTHM_FACTORYINIT_H
