//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef LIFERHYTHM_ERROR_H
#define LIFERHYTHM_ERROR_H

#include <exception>
#include <QString>

#define throwInFunc(what)  throw Error(__FILE__ + QString("::") + __FUNCTION__, what)

class Error : public std::exception {
public:
    const QString info;

public:
    explicit Error(const QString& where);
    explicit Error(const QString& where, const QString& what);
    explicit Error(const QString& cause, const QString& where, const QString& what);
    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
};

#endif //LIFERHYTHM_ERROR_H
