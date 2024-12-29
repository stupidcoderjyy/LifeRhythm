//
// Created by stupid_coder_jyy on 2023/12/28.
//

#include "PrintErrorHandler.h"
#include <QDebug>

USING_LR

void PrintErrorHandler::onErrorCaught(Error &err) {
    qDebug() << err.why.toUtf8().data();
}
