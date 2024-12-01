//
// Created by stupid_coder_jyy on 2024/1/16.
//

#include "FactoryInit.h"

#include "Button.h"
#include "Widget.h"
#include "FocusContainer.h"
#include "LineEdit.h"
#include "Menu.h"
// #include "TextEdit.h"
// #include "ColorSelector.h"
// #include "Separator.h"

#define reg(T) stdSuppliers->insert(#T, [](QWidget* p){ return new T(p, false);}); \
    stdEmptyInstances->insert(#T, new T(nullptr, false));

void FactoryInit::mainInit(QMap<QString, WidgetFactory::Supplier> *stdSuppliers,
                           QMap<QString, StandardWidget *> *stdEmptyInstances) {
    // reg(Widget)
    // reg(Button)
    // reg(Label)
    // reg(FocusContainer)
    // reg(LineEdit)
    // reg(Menu)
    // reg(ColorIcon)
    // reg(DefaultColorsList)
    // reg(HueSelector)
    // reg(VColorBar)
    // reg(Separator)
}