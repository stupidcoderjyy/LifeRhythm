//
// Created by stupid_coder_jyy on 2024/1/15.
//

#ifndef LIFERHYTHM_QSSPARSER_H
#define LIFERHYTHM_QSSPARSER_H

#include <QMap>
class CompilerInput;

class QssItem {
    friend class QssParser;
protected:
    QString key;
    virtual QString translate(const QStringList& args) = 0;
    explicit QssItem(QString key);
};

class QssParser {
private:
    static QMap<QString, QssItem*> items;
public:
    static void init();
    static QString translate(const QString& expr);
    static void addStringConcatItem(const QString& key, QString prefix = "", QString suffix = "");
    static void addStringReplaceItem(const QString& key, QString value);
private:
    static QString parseBlock(CompilerInput* input);
    static QString parseItem(CompilerInput* input);
    static QStringList parseArgs(CompilerInput* input);
};

class StringReplaceItem : QssItem {
    friend class QssParser;
private:
    QString value;
protected:
    explicit StringReplaceItem(QString key, QString value);
    QString translate(const QStringList &args) override;
};

class StringConcatItem : public QssItem {
    friend class QssParser;
private:
    QString prefix;
    QString suffix;
protected:
    explicit StringConcatItem(const QString& key, QString prefix = "", QString suffix = "");
    QString translate(const QStringList &args) override;
};

#endif //LIFERHYTHM_QSSPARSER_H
