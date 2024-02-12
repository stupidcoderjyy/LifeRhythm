//
// Created by stupid_coder_jyy on 2024/1/16.
//

#include "FactoryInit.h"
#include "widgets/Widget.h"
#include "widgets/ImgButton.h"
#include "widgets/TextLabel.h"

#define reg(T) stdSuppliers->insert(#T, [](QWidget* p){ return new T(p);}); \
    stdEmptyInstances->insert(#T, new T(nullptr));

void FactoryInit::init(QMap<QString, WidgetFactory::Supplier> *stdSuppliers,
                       QMap<QString, StandardWidget *> *stdEmptyInstances) {
    reg(Widget)
    reg(ImgLabel)
    reg(ImgButton)
    reg(TextLabel)
}
