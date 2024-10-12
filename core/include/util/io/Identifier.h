//
// Created by stupid_coder_jyy on 2024/1/14.
//

#ifndef LIFERHYTHM_IDENTIFIER_H
#define LIFERHYTHM_IDENTIFIER_H

#include <QDir>

#define LOC(x) Identifier(x)

class Identifier{
private:
    QString _namespace;
    QString _path;

public:
    Identifier(QString _namespace, QString _path);
    explicit Identifier(const QString& _path);
    explicit Identifier(const char *_path);
    Identifier(const Identifier& o);
    Identifier(Identifier&& o) noexcept;
    Identifier& operator=(const Identifier& o);
    bool operator==(const Identifier &rhs) const;
    bool operator!=(const Identifier &rhs) const;
    bool operator<(const Identifier &rhs) const;
    bool operator>(const Identifier &rhs) const;
    bool operator<=(const Identifier &rhs) const;
    bool operator>=(const Identifier &rhs) const;
    QString toString() const;
    Identifier withSuffix(const QString& suffix) const;
    Identifier withPrefix(const QString& prefix) const;
    QString toFullPath() const;
    QString getPath() const;
    QString getNamespace() const;
};

#endif //LIFERHYTHM_IDENTIFIER_H
