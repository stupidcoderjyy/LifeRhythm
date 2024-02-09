//
// Created by stupid_coder_jyy on 2024/1/16.
//

#include "FactoryInit.h"
#include "StdWidget.h"
#include "StdImgButton.h"
#include "StdTextLabel.h"

#define reg(name, T) stdSuppliers->insert(name, [](QWidget* p){ return new T(p);}); \
    stdEmptyInstances->insert(name, new T(nullptr));

void FactoryInit::init(QMap<QString, WidgetFactory::Supplier> *stdSuppliers,
                       QMap<QString, StandardWidget *> *stdEmptyInstances) {
    reg("std_widget", StdWidget)
    reg("std_imgLabel", StdImgLabel)
    reg("std_imgButton", StdImgButton)
    reg("std_textLabel", StdTextLabel)
}
