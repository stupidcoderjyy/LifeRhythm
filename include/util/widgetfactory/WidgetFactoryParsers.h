//
// Created by stupid_coder_jyy on 2024/1/17.
//

#ifndef LIFERHYTHM_WIDGETFACTORYPARSERS_H
#define LIFERHYTHM_WIDGETFACTORYPARSERS_H

#include <QString>
#include <QMap>
#include <QSizePolicy>
#include <QMargins>

class ArrayData;

class WidgetFactoryParsers{
    friend class WidgetFactory;
private:
    static QMap<QString, Qt::AlignmentFlag> alignments;
    static QMap<QString,QSizePolicy::Policy> policies;
public:
    static Qt::Alignment parseAlign(const QString& alignment);
    static QMargins parseMargins(ArrayData* array);
private:
    static void init();
};



#endif //LIFERHYTHM_WIDGETFACTORYPARSERS_H
