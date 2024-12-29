//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H
#include "Namespaces.h"

BEGIN_LR

template<class T>
class ErrorHandler {
public:
    virtual void onErrorCaught(T& err) = 0;
    virtual ~ErrorHandler() = default;
};

END_LR

#endif //ERRORHANDLER_H
