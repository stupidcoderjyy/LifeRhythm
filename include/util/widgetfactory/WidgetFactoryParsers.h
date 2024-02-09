//
// Created by stupid_coder_jyy on 2024/1/17.
//

#ifndef LIFERHYTHM_WIDGETFACTORYPARSERS_H
#define LIFERHYTHM_WIDGETFACTORYPARSERS_H

#include <QString>
#include <QMap>
#include <QSizePolicy>
#include <QMargins>
#include "WidgetFactory.h"
#include "NBT.h"
#include "Styles.h"

class ArrayData;

class WidgetFactoryParsers{
    friend class WidgetFactory;
private:
    static QMap<QString, Qt::AlignmentFlag> alignments;
    static QMap<QString, QSizePolicy::Policy> policies;
    static QMap<QString, QString> colors;
public:
    static Qt::Alignment parseAlign(const QString& alignment);
    static void parseSizePolicy(WidgetFactory::Handlers& handlers, NBT* nbt);
    static QMargins parseMargins(ArrayData* array);
    template<class W> static void parseTextWidget(WidgetFactory::Handlers& handlers, NBT* nbt) {
        QString text = nbt->getString("text", "");
        QString fontFamily = nbt->getString("font_family", Styles::MAIN_FONT);
        int fontSize = nbt->getInt("font_size", Styles::FONT_SIZE_MEDIUM);
        bool bold = nbt->getBool("bold");
        Qt::Alignment align = parseAlign(nbt->getString("align"));
        QColor color = parseColor(nbt->getString("color", Styles::GRAY_TEXT_0));
        handlers << [text,fontFamily,fontSize,bold,align,color](QWidget* target) {
            auto* textWidget = static_cast<W*>(target);
            textWidget->setText(text);
            QFont f = textWidget->font();
            f.setFamily(fontFamily);
            f.setPointSize(fontSize);
            f.setBold(bold);
            f.setStyleStrategy(QFont::PreferAntialias);
            QPalette p = textWidget->palette();
            p.setColor(QPalette::WindowText, color);
            p.setColor(QPalette::Text, color);
            textWidget->setPalette(p);
            textWidget->setFont(f);
            textWidget->setAlignment(align);
        };
    }
    static QSize parseSize(ArrayData* arr);
    static QColor parseColor(const QString& str);
private:
    static void init();
};



#endif //LIFERHYTHM_WIDGETFACTORYPARSERS_H
