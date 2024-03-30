//
// Created by stupid_coder_jyy on 2024/3/30.
//

#ifndef LIFERHYTHM_MODULELOG_H
#define LIFERHYTHM_MODULELOG_H

#include "Namespaces.h"
#include "Module.h"

BEGIN_NAMESPACE(lr::log)

class WidgetA;

class ModuleLog : public Module {
private:
    WidgetA* tabMain;
public:
    ModuleLog();
protected:
    void mainInit() override;
    void postInit() override;
};

END_NAMESPACE

#endif //LIFERHYTHM_MODULELOG_H
