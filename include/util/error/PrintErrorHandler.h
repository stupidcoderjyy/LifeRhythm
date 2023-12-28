//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef LIFERHYTHM_PRINTERRORHANDLER_H
#define LIFERHYTHM_PRINTERRORHANDLER_H
#include "ErrorHandler.h"

class PrintErrorHandler : public ErrorHandler{
public:
    void onErrorCaught(const Error &err) override;
};
#endif //LIFERHYTHM_PRINTERRORHANDLER_H
