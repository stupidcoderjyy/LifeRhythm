//
// Created by stupid_coder_jyy on 2024/2/11.
//

#ifndef LIFERHYTHM_CONFIG_H
#define LIFERHYTHM_CONFIG_H

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

class Config {
private:
    QString savePath{};
    bool frozen{};
public:
    const QString &getSavePath() const;
    void setSavePath(const QString &savePath);
    void froze();
};

LR_END


#endif //LIFERHYTHM_CONFIG_H
