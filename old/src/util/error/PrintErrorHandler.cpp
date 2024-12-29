//
// Created by stupid_coder_jyy on 2023/12/28.
//

#include "PrintErrorHandler.h"
#include <QDebug>

void PrintErrorHandler::onErrorCaught(Error &err) {
    qDebug() << ('[' + err.where + ']' + err.why).toUtf8().data();
}
