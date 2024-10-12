//
// Created by stupid_coder_jyy on 2024/3/30.
//

#ifndef LIFERHYTHM_MODULELOG_H
#define LIFERHYTHM_MODULELOG_H

#include "Namespaces.h"
#include "Module.h"

BEGIN_NAMESPACE(lr::log)

class A;
class B;

class ModuleLog final : public Module {
private:
    A* tabMain;
    B* tabProfiles;
public:
    ModuleLog();
protected:
    void mainInit() override;
    void postInit() override;
};

END_NAMESPACE

#endif //LIFERHYTHM_MODULELOG_H
