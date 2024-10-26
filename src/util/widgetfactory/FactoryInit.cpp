//
// Created by stupid_coder_jyy on 2024/1/16.
//

#include "FactoryInit.h"
#include "Widget.h"
#include "ImgButton.h"
#include "TextLabel.h"
#include "FocusContainer.h"
#include "TextEdit.h"
#include "LineEdit.h"
#include "Menu.h"
#include "ArrowButton.h"
#include "TextButton.h"
#include "ColorSelector.h"
#include "IconTextButton.h"
#include "Separator.h"

#define reg(T) stdSuppliers->insert(#T, [](QWidget* p){ return new T(p);}); \
    stdEmptyInstances->insert(#T, new T(nullptr));

void FactoryInit::mainInit(QMap<QString, WidgetFactory::Supplier> *stdSuppliers,
                           QMap<QString, StandardWidget *> *stdEmptyInstances) {
    reg(Widget)
    reg(ImgLabel)
    reg(ImgButton)
    reg(TextLabel)
    reg(FocusContainer)
    reg(TextEdit)
    reg(LineEdit)
    reg(Menu)
    reg(ArrowButton)
    ArrowButton::mainInit();
    reg(TextButton)
    reg(ColorIcon)
    reg(DefaultColorsList)
    reg(HueSelector)
    reg(VColorBar)
    reg(IconTextButton)
    reg(Separator)
}