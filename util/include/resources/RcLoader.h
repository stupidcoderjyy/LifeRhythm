//
// Created by JYY on 24-12-24.
//

#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include "Identifier.h"
#include "Plugin.h"
#include "Namespaces.h"

#define SINGLETON_HEADER(C)\
    protected:\
        C() = default;\
    public:\
        static C* get() {  \
            static auto* instance = new C; \
            return instance;\
        };

BEGIN_LR

template<class T>
class CORE_API RcLoader {
public:
    virtual ~RcLoader() = default;
    virtual T* load(const Identifier& loc, const QString& absolutePath) = 0;
    virtual void onLoadFailed(std::exception& e) noexcept {}
    virtual void deleteElement(T* p) { delete p; }
};

END_LR

#endif //RESOURCELOADER_H
