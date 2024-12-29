//
// Created by stupid_coder_jyy on 2023/12/28.
//

#include <Error.h>
#include <utility>

USING_LR

Error::Error(QString why): why(std::move(why)) {
}

const char *Error::what() const noexcept {
    return why.toStdString().c_str();
}
