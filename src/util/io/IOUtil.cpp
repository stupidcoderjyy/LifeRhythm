//
// Created by stupid_coder_jyy on 2023/5/31.
//
#include "IOUtil.h"
#include "windows.h"
#include <iostream>
#include "QDir"

void setConsoleColor(int foreground) {
    WORD wColor = (foreground & 0x0Fu);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
}

void printWhite(const QString &msg) {
    setConsoleColor(CONSOLE_WHITE);
    std::cout << msg.toStdString();
}

void printRed(const QString &msg) {
    setConsoleColor(CONSOLE_RED);
    std::cout << msg.toStdString();
}

void printYellow(const QString &msg) {
    setConsoleColor(CONSOLE_BROWN);
    std::cout << msg.toStdString();
}

void printBlue(const QString &msg) {
    setConsoleColor(CONSOLE_LIGHT_BLUE);
    std::cout << msg.toStdString();
}

void checkFolders(const QString &folderPath) {
    QDir d(folderPath);
    if (!d.exists()) {
        d.mkpath(folderPath);
    }
}

QString concatPath(const QString& path1, const QString& path2, const QString& path3) {
    if (path1.isEmpty()) {
        return concatPath(path2, path3);
    }
    if (path2.isEmpty()) {
        return concatPath(path1, path3);
    }
    if (path3.isEmpty()) {
        return concatPath(path1, path2);
    }
    return path1 + "/" + path2 + "/" + path3;
}

QString concatPath(const QString& path1, const QString& path2) {
    if (path1.isEmpty()) {
        return path2;
    }
    if (path2.isEmpty()) {
        return path1;
    }
    return path1 + "/" + path2;
}
