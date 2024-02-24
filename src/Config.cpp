//
// Created by stupid_coder_jyy on 2024/2/11.
//

#include "Config.h"
#include "Error.h"

USING_LR

Version::Version(int major, int minor): major(major), minor(minor) {
}

Version::Version(const Version &o) {
    major = o.major;
    minor = o.minor;
}

Version::Version(Version &&o) noexcept {
    major = o.major;
    minor = o.minor;
}

bool Version::operator==(const Version &rhs) const {
    return major == rhs.major && minor == rhs.minor;
}

bool Version::operator!=(const Version &rhs) const {
    return !(rhs == *this);
}

bool Version::operator<(const Version &rhs) const {
    if (major < rhs.major)
        return true;
    if (major > rhs.major)
        return false;
    return minor < rhs.minor;
}

bool Version::operator>(const Version &rhs) const {
    return rhs < *this;
}

bool Version::operator<=(const Version &rhs) const {
    return !(rhs < *this);
}

bool Version::operator>=(const Version &rhs) const {
    return !(*this < rhs);
}

QString Version::toString() const {
    return QString::number(major) + "." + QString::number(minor);
}

const QString &Config::getSavePath() const {
    return savePath;
}

void Config::setSavePath(const QString &path) {
    if (frozen) {
        throwInFunc("frozen");
    }
    Config::savePath = path;
}

void Config::froze() {
    frozen = true;
}
