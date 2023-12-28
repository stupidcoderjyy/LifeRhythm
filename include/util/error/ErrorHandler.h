//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef LIFERHYTHM_ERRORHANDLER_H
#define LIFERHYTHM_ERRORHANDLER_H

#include <Error.h>

class ErrorHandler {
public:
    virtual void onErrorCaught(const Error& err) = 0;
};

#endif //LIFERHYTHM_ERRORHANDLER_H
