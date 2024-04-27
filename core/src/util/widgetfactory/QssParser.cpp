//
// Created by stupid_coder_jyy on 2024/1/15.
//

#include "QssParser.h"
#include "Styles.h"
#include "CompilerInput.h"
#include <utility>
#include <QStringBuilder>
#include <QDebug>

QssItem::QssItem(QString key):key(std::move(key)) {
}

QMap<QString, QssItem*> QssParser::items{};

QString QssParser::translate(const QString& expr) {
    QString result{};
    auto* input = CompilerInput::fromString(expr);
    try {
        while (input->available()) {
            result = result % parseBlock(input);
        }
    } catch (CompileError& e) {
        e.printErr();
        result = "";
    }
    delete input;
    return result;
}

QString QssParser::parseBlock(CompilerInput *input) {
    input->skip(' ');
    if (!input->available()) {
        return "";
    }
    int ch = input->read();
    switch (ch) {
        case '\'': {
            //常量
            input->mark();
            int end = input->approach('\'');
            input->mark();
            if (end != '\'') {
                throw input->errorAtForward("missing '''");
            }
            QString res = input->capture();
            input->read();
            return res;
        }
        default:
            input->retract();
            return parseItem(input);
        case ')':
            input->retract();
            break;
    }
    return "";
}

QString QssParser::parseItem(CompilerInput *input) {
    input->mark();
    int end = input->approach('(', ',', ')');
    input->mark();
    QString key = input->capture();
    if (!items.contains(key)) {
        throw input->errorAtForward("invalid item key '" + key + "'");
    }
    QStringList args = end == '(' ?
                       parseArgs(input) :
                       QStringList();
    return items.value(key)->translate(args);
}

QString QssParser::parseArg(CompilerInput *input) {
    QString res{};
    while(true) {
        input->skip(' ');
        if (!input->available()) {
            return res;
        }
        int next = input->read();
        input->retract();
        if (next == ',' || next == ')') {
            return res;
        }
        res = res % parseBlock(input);
    }
}

QStringList QssParser::parseArgs(CompilerInput *input) {
    input->read(); //(
    QStringList list;
    list << parseArg(input);
    while (true) {
        switch (input->find(',', ')')) {
            case ',': {
                list << parseArg(input);
                break;
            }
            case ')': {
                return list;
            }
            case -1: {
                throw input->errorAtForward("missing ')'");
            }
        }
    }
}

void QssParser::mainInit() {
    addStringConcatItem("bg", "background-color:", ";");
    addStringConcatItem("brad", "border-radius:", ";");
    for (auto& c : Color::defaultColors) {
        addStringReplaceItem(c->name, c->rgbHex);
    }
    registerItem(new BorderItem("bd", "border"));
    registerItem(new BorderItem("bd_b", "border-bottom"));
    registerItem(new BorderItem("bd_t", "border-top"));
    registerItem(new BorderItem("bd_l", "border-left"));
    registerItem(new BorderItem("bd_r", "border-right"));
}

void QssParser::addStringConcatItem(const QString& key, QString prefix, QString suffix) {
    registerItem(new StringConcatItem(key, std::move(prefix), std::move(suffix)));
}

void QssParser::addStringReplaceItem(const QString &key, QString value) {
    registerItem(new StringReplaceItem(key, std::move(value)));
}

void QssParser::registerItem(QssItem *item) {
    items.insert(item->key, item);
}

StringReplaceItem::StringReplaceItem(QString key, QString value) :
        QssItem(std::move(key)),
        value(std::move(value)){
}

QString StringReplaceItem::translate(const QStringList &args) {
    return value;
}

StringConcatItem::StringConcatItem(QString key, QString prefix, QString suffix) :
        QssItem(std::move(key)),
        prefix(std::move(prefix)),
        suffix(std::move(suffix)){
}

QString StringConcatItem::translate(const QStringList &args) {
    if (args.empty()) {
        return prefix + suffix;
    }
    return prefix + args[0] + suffix;
}

BorderItem::BorderItem(const QString& key, QString propName):
        QssItem(key),propName(std::move(propName)){
}

QString BorderItem::translate(const QStringList &args) {
    QString res = propName + ":";
    for (auto& arg : args) {
        res = res % arg % ' ';
    }
    return res + ";";
}