//
// Created by stupid_coder_jyy on 2024/1/15.
//

#ifndef QSSPARSER_H
#define QSSPARSER_H

#include <QMap>

#include "Namespaces.h"
#include "Plugin.h"

BEGIN_LR

class CompilerInput;

class CORE_API QssItem {
private:
    friend class QssParser;
public:
    virtual ~QssItem() = default;
protected:
    QString key;
    virtual QString translate(const QStringList& args) = 0;
    explicit QssItem(QString key);
};

class CORE_API QssParser {
private:
    static QMap<QString, QssItem*> items;
public:
    static void mainInit();
    static QString translate(const QString& expr);
    static void addStringConcatItem(const QString& key, QString prefix = "", QString suffix = "");
    static void addStringReplaceItem(const QString& key, QString value);
    static void registerItem(QssItem* item);
private:
    static QString parseBlock(CompilerInput* input);
    static QString parseItem(CompilerInput* input);
    static QString parseArg(CompilerInput* input);
    static QStringList parseArgs(CompilerInput* input);
};

class CORE_API StringReplaceItem : public QssItem {
private:
    QString value;
public:
    explicit StringReplaceItem(QString key, QString value);
    QString translate(const QStringList &args) override;
};

class CORE_API StringConcatItem : public QssItem {
private:
    QString prefix;
    QString suffix;
public:
    explicit StringConcatItem(QString key, QString prefix = "", QString suffix = "");
    QString translate(const QStringList &args) override;
};

class CORE_API BorderItem: public QssItem{
private:
    QString propName;
public:
    explicit BorderItem(const QString& key, QString propName);
protected:
    QString translate(const QStringList &args) override;
};

END_LR

#endif //QSSPARSER_H
