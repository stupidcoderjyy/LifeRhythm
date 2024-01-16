//
// Created by stupid_coder_jyy on 2024/1/14.
//
#include "Identifier.h"

#include <utility>

Identifier::Identifier(QString _namespace, QString _path):_namespace(std::move(_namespace)),_path(std::move(_path)) {
}

Identifier::Identifier(const QString& _path) {
    int i = _path.indexOf(':');
    if (i < 0) {
        this->_namespace = "lr";
        this->_path = _path;
    } else {
        this->_namespace = _path.mid(0, i);
        this->_path = _path.mid(i + 1, _path.length() - i);
    }
}

Identifier::Identifier(const char *_path):Identifier(QString(_path)) {
}

Identifier::Identifier(const Identifier &o) {
    _namespace = o._namespace;
    _path = o._path;
}

Identifier::Identifier(Identifier &&o) noexcept {
    _namespace = std::move(o._namespace);
    _path = std::move(o._path);
}

Identifier &Identifier::operator=(const Identifier &o) {
    if (this != &o) {
        _namespace = o._namespace;
        _path = o._path;
    }
    return *this;
}

bool Identifier::operator==(const Identifier &rhs) const {
    return _namespace == rhs._namespace &&
           _path == rhs._path;
}

bool Identifier::operator!=(const Identifier &rhs) const {
    return !(rhs == *this);
}

QString Identifier::toString() const {
    return _namespace + ":" + _path;
}

Identifier Identifier::withSuffix(const QString &suffix) const {
    return {_namespace, _path + suffix};
}

Identifier Identifier::withPrefix(const QString &prefix) const {
    return {_namespace, prefix + _path};
}

QString Identifier::toFullPath() const {
    return ":/assets/" + _namespace + "/" + _path;
}

QString Identifier::getPath() const {
    return _path;
}

QString Identifier::getNamespace() const {
    return _namespace;
}

QFile Identifier::toFile() const {
    return toFullPath();
}

QDir Identifier::toDir() const {
    return toFullPath();
}

bool Identifier::operator<(const Identifier &rhs) const {
    if (_namespace < rhs._namespace)
        return true;
    if (rhs._namespace < _namespace)
        return false;
    return _path < rhs._path;
}

bool Identifier::operator>(const Identifier &rhs) const {
    return rhs < *this;
}

bool Identifier::operator<=(const Identifier &rhs) const {
    return !(rhs < *this);
}

bool Identifier::operator>=(const Identifier &rhs) const {
    return !(*this < rhs);
}

QString Identifier::absolutePath(const QString &_namespace, const QString &path1, const QString &path2) {
    QString res = ":/assets/" + _namespace;
    if (path1.isEmpty()) {
        return path2.isEmpty() ? res : res + "/" + path2;
    }
    res += "/" + path1;
    return path2.isEmpty() ? res : res + "/" + path2;
}
