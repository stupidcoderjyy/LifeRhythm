//
// Created by stupid_coder_jyy on 2024/1/16.
//

#ifndef FACTORYINIT_H
#define FACTORYINIT_H

#include "WidgetFactory.h"

BEGIN_LR

class CORE_API FactoryInit {
    friend class WidgetFactory;
private:
    static void mainInit(QMap<QString, WidgetFactory::Supplier>* stdSuppliers,
                         QMap<QString, StandardWidget*>* stdEmptyInstances);
};

END_LR


#endif //FACTORYINIT_H
