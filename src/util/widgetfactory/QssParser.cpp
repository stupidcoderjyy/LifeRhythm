//
// Created by stupid_coder_jyy on 2024/1/15.
//

#include "QssParser.h"
#include "Styles.h"
#include "CompilerInput.h"
#include <utility>
#include <QStringBuilder>

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
        case '#':
            return parseItem(input);
        case ')':
            input->retract();
            break;
        default: {
            input->retract();
            throw input->errorAtForward(QString("unexpected symbol '") + QChar(ch) + "'");
        }
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

void QssParser::init() {
    addStringConcatItem("bg", "background-color:", ";");
    addStringReplaceItem("GRAY_0", Styles::GRAY_0);
    addStringReplaceItem("GRAY_1", Styles::GRAY_1);
    addStringReplaceItem("GRAY_2", Styles::GRAY_2);
    addStringReplaceItem("GRAY_3", Styles::GRAY_3);
    addStringReplaceItem("GRAY_4", Styles::GRAY_4);
    addStringReplaceItem("BLACK", Styles::BLACK);
    items.insert("border", new BorderItem());
    items.insert("target", new TargetItem());
}

void QssParser::addStringConcatItem(const QString& key, QString prefix, QString suffix) {
    items.insert(key, new StringConcatItem(key, std::move(prefix), std::move(suffix)));
}

void QssParser::addStringReplaceItem(const QString &key, QString value) {
    items.insert(key, new StringReplaceItem(key, std::move(value)));
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

BorderItem::BorderItem():
        QssItem("border"){
}

QString BorderItem::translate(const QStringList &args) {
    QString res{};
    if (!args.empty()) {
        res = res % "border-style:" % args[0] % ';';
    }
    if (args.length() > 1) {
        res = res % "border-width:" % args[1] % ';';
    }
    if (args.length() > 2) {
        res = res % "border-color:" % args[2] % ';';
    }
    return res;
}

TargetItem::TargetItem():QssItem("target") {
}

QString TargetItem::translate(const QStringList &args) {
    if (args.empty()) {
        return {};
    }
    QString res = '#' + args[0] + '{';
    if (args.length() > 1) {
        res += args[1];
    }
    res += '}';
    return res;
}
