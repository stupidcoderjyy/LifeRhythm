//
// Created by stupid_coder_jyy on 2024/2/14.
//

#ifndef LIFERHYTHM_RCLOADER_H
#define LIFERHYTHM_RCLOADER_H

#include "Identifier.h"

template<class T>
class RcLoader {
public:
    virtual T* load(const Identifier& loc, const QString& absolutePath) = 0;
    virtual void onLoadFailed(std::exception& e) noexcept {};
    virtual void deleteElement(T* p) { delete p; }
};

#define SINGLETON_HEADER(C)\
    protected:\
        C() = default;\
    public:\
        static C* get() {  \
            static auto* instance = new C; \
            return instance;\
        };

#endif //LIFERHYTHM_RCLOADER_H
