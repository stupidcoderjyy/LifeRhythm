//
// Created by stupid_coder_jyy on 2024/2/11.
//

#ifndef LIFERHYTHM_CONSTANTS_H
#define LIFERHYTHM_CONSTANTS_H

#include <QString>
#include "Namespaces.h"

LR_BEGIN

class Version {
private:
    int major;
    int minor;
public:
    Version(int major, int minor);
    Version(const Version& o);
    Version(Version&& o) noexcept;
    QString toString() const;
    bool operator==(const Version &rhs) const;
    bool operator!=(const Version &rhs) const;
    bool operator<(const Version &rhs) const;
    bool operator>(const Version &rhs) const;
    bool operator<=(const Version &rhs) const;
    bool operator>=(const Version &rhs) const;
};

class Constants {
public:
    static const QString NAME;
    static const Version API_VERSION;
};

LR_END


#endif //LIFERHYTHM_CONSTANTS_H
