//
// Created by stupid_coder_jyy on 2024/1/16.
//

#include "FactoryInit.h"

USING_LR

#define reg(T) stdSuppliers->insert(#T, [](QWidget* p){ return new T(p, false);}); \
    stdEmptyInstances->insert(#T, new T(nullptr, false));

void FactoryInit::mainInit(QMap<QString, WidgetFactory::Supplier> *stdSuppliers,
                           QMap<QString, StandardWidget *> *stdEmptyInstances) {
}