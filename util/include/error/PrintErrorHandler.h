//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef PRINTERRORHANDLER_H
#define PRINTERRORHANDLER_H
#include "ErrorHandler.h"
#include "Error.h"

BEGIN_LR

class CORE_API PrintErrorHandler : public ErrorHandler<Error> {
public:
    void onErrorCaught(Error &err) override;
};

END_LR

#endif //PRINTERRORHANDLER_H
