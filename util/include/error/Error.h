//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef ERROR_H
#define ERROR_H

#include <exception>
#include <QString>

#include "Namespaces.h"
#include "Plugin.h"

BEGIN_LR

class CORE_API Error : public std::exception {
public:
    QString why;
public:
    explicit Error(QString why);
    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
};

END_LR

#endif //ERROR_H
