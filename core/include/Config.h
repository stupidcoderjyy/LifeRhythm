//
// Created by stupid_coder_jyy on 2024/2/11.
//

#ifndef LIFERHYTHM_CONFIG_H
#define LIFERHYTHM_CONFIG_H

#include <QString>
#include "Namespaces.h"

BEGIN_NAMESPACE(lr)

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
    friend class LifeRhythm;
public:
    enum Mode {
        Normal,
        Test
    };
private:
    Mode mode;
    QString savePath;
    bool frozen;
public:
    Config();
    Config(const Config& o);
    const QString &getSavePath() const;
    void setSavePath(const QString &savePath);
    Mode getMode() const;
    void setMode(Mode mode);
    void froze();
};

END_NAMESPACE

#endif //LIFERHYTHM_CONFIG_H
