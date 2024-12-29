//
// Created by JYY on 24-10-21.
//

#ifndef PLUGIN_H
#define PLUGIN_H

#include "qcompilerdetection.h"

#define PLUGIN_ENTRYPOINT extern "C" __declspec(dllexport) void pluginEntryPoint()

#ifdef EXPORT_LIB
#define CORE_API Q_DECL_EXPORT
#else
#ifdef CORE_LOCAL_COMPILE
#define CORE_API
#else
#define CORE_API Q_DECL_IMPORT
#endif
#endif

#endif //PLUGIN_H
