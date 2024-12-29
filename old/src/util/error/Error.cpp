//
// Created by stupid_coder_jyy on 2023/12/28.
//

#include <Error.h>

#include <utility>

Error::Error(QString where): where(std::move(where)), why("") {
}

Error::Error(QString where, QString what): where(std::move(where)), why(std::move(what)) {
}

const char *Error::what() const noexcept {
    return where.toStdString().c_str();
}
