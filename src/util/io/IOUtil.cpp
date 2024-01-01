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