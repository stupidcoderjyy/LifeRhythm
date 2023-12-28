//
// Created by stupid_coder_jyy on 2023/12/28.
//

#include <Error.h>

Error::Error(const QString& where): info("[" + where + "]") {

}

Error::Error(const QString& where, const QString& what):info("[" + where + "]" + what) {

}

Error::Error(const QString &cause, const QString &where, const QString &what)
:info("[" + cause + "][" + where + "] " + what) {
}

const char *Error::what() const noexcept {
    return "";
}
