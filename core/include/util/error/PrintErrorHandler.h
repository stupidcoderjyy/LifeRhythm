//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef LIFERHYTHM_PRINTERRORHANDLER_H
#define LIFERHYTHM_PRINTERRORHANDLER_H
#include "ErrorHandler.h"
#include "Error.h"

class CORE_API PrintErrorHandler : public ErrorHandler<Error> {
public:
    void onErrorCaught(Error &err) override;
};
#endif //LIFERHYTHM_PRINTERRORHANDLER_H
