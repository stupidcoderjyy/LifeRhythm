//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef LIFERHYTHM_ERRORHANDLER_H
#define LIFERHYTHM_ERRORHANDLER_H

template<class T>
class ErrorHandler {
public:
    virtual void onErrorCaught(T& err) = 0;
    virtual ~ErrorHandler() = default;
};

#endif //LIFERHYTHM_ERRORHANDLER_H
