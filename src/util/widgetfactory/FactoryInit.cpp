//
// Created by stupid_coder_jyy on 2024/1/16.
//

#include "FactoryInit.h"
#include "StdWidget.h"

void FactoryInit::init(QMap<QString, WidgetFactory::Supplier> *stdSuppliers,
                       QMap<QString, StandardWidget *> *stdEmptyInstances) {
    stdSuppliers->insert("std_widget", [](QWidget* p){ return new StdWidget(p);});

    stdEmptyInstances->insert("std_widget", new StdWidget(nullptr));
}
