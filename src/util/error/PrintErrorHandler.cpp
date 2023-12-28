//
// Created by stupid_coder_jyy on 2023/12/28.
//

#include "PrintErrorHandler.h"
#include <QDebug>

void PrintErrorHandler::onErrorCaught(const Error &err) {
    qDebug() << err.info;
}
