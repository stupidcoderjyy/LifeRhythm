//
// Created by stupid_coder_jyy on 2024/1/14.
//

#ifndef LIFERHYTHM_WIDGETFACTORY_H
#define LIFERHYTHM_WIDGETFACTORY_H

#include <QSpacerItem>
#include "StandardWidget.h"
#include "Identifier.h"
#include "NBT.h"
#include "Styles.h"

#define regClazz(factory, T) factory->registerStdWidget(#T, [](QWidget* p){ return new T(p); }, new T(nullptr))

class NBT;

class CORE_API WidgetFactory final {
public:
    typedef std::function<QWidget*(QWidget* parent)> Supplier;
    typedef StandardWidget::Handlers Handlers;
private:
    static QRegExp regexId;
    static QMap<QString, Supplier> stdSuppliers;
    static QMap<QString, StandardWidget*> stdEmptyInstances;
    static QMap<QString, Qt::AlignmentFlag> alignments;
    static QMap<QString, QSizePolicy::Policy> policies;
    enum State{
        Empty,
        Parsing,
        Ready,
        Broken
    };
    QMap<QString, Supplier>* customSuppliers;
    QMap<QString, StandardWidget*>* customEmptyInstances;
    QMap<QString, WidgetFactory*> childFactories;
    QMap<int, Handlers> stateResponders;
    Handlers globalResponders;
    WidgetFactory* parentFactory;
    Handlers handlers{};
    QWidget* workingWidget{};
    StandardWidget* stdType{};
    QString nbtPath{};
    NBT* source{};
    State state = Empty;
    QString id;
public:
    static void mainInit();
    static WidgetFactory* fromResource(const Identifier& loc);
    static WidgetFactory* fromNbt(const QString& id, NBT* nbt);
    template<class W> static void parseTextWidget(Handlers& handlers, NBT* nbt);
    static Qt::Alignment parseAlign(const QString& align);
    static QSize parseSize(ArrayData* arr);
    void setSource(NBT* nbt);
    void parse() noexcept;
    void registerStdWidget(const QString& type, const Supplier& supplier, StandardWidget* instance) const;
    QWidget* apply(QWidget* parent = nullptr, QWidget* target = nullptr) noexcept;
    template<class T> T* applyAndCast(QWidget* parent = nullptr, QWidget* target = nullptr) {
        return static_cast<T*>(apply(parent, target));
    }
    ~WidgetFactory();
private:
    static void parseQss(Handlers& target, NBT* nbt);
    static void parseSize(Handlers& op, NBT* nbt);
    static QMargins parseMargins(ArrayData* array);
    static void parseSizePolicy(Handlers& op, NBT* nbt);
    static QSizePolicy::Policy parsePolicy(const QString& name);
    explicit WidgetFactory(QString id);
    explicit WidgetFactory(WidgetFactory* parent, const QString& id, NBT* nbt);
    StandardWidget* parseWidgetType(NBT* nbt);
    void parseChildren(NBT* nbt);
    WidgetFactory* findFactory(NBT* nbt, const QString& path);
    void parseLayout(NBT* target);
    void parseSpacers(NBT* nbt);
    void parseGridLayout(NBT* nbt);
    void parseMargins(NBT* nbt);
    void parseStates(NBT* nbt);
    void parseSingleState(Handlers& op, NBT* stateTag) const;
    void parsePointer(NBT* nbt);
    QWidget* createWidget(const QString& type) const;
    void parseModel(NBT* nbt);
};

template<class W>
void WidgetFactory::parseTextWidget(Handlers &handlers, NBT *nbt) {
    if (nbt->contains("text", Data::STRING)) {
        QString text = nbt->getString("text", "");
        handlers << [text](QWidget* target) {
            static_cast<W*>(target)->setText(text);
        };
    }
    if (nbt->contains("bold", Data::BOOL)) {
        bool bold = nbt->getBool("bold");
        handlers << [bold](QWidget* target) {
            auto* tw = static_cast<W*>(target);
            QFont f = tw->font();
            f.setBold(bold);
            tw->setFont(f);
        };
    }
    if (nbt->contains("ff")) {
        auto* data = nbt->get("ff");
        if (data->type == Data::STRING) {
            QString ff = data->asString()->get();
            handlers << [ff](QWidget* target) {
                auto* tw = static_cast<W*>(target);
                QFont f = tw->font();
                f.setFamilies({});
                f.setFamily(ff);
                tw->setFont(f);
            };
        } else if (data->type == Data::ARR) {
            QStringList ff;
            data->asArray()->fillString(ff);
            handlers << [ff](QWidget* target) {
                auto* tw = static_cast<W*>(target);
                QFont f = tw->font();
                f.setFamilies(ff);
                tw->setFont(f);
            };
        }
    }
    if (nbt->contains("fs", Data::INT)) {
        int fs = nbt->getInt("fs", Styles::FS_MEDIUM);
        handlers << [fs](QWidget* target) {
            auto* tw = static_cast<W*>(target);
            QFont f = tw->font();
            f.setPointSize(fs);
            tw->setFont(f);
        };
    }
    if (nbt->contains("align", Data::STRING)) {
        Qt::Alignment a = parseAlign(nbt->getString("align"));
        handlers << [a](QWidget* target) {
            static_cast<W*>(target)->setAlignment(a);
        };
    }
    if (nbt->contains("color", Data::STRING)) {
        QColor c = Styles::parseColor(nbt->getString("color", Styles::GRAY_TEXT_0->name));
        handlers << [c](QWidget* target) {
            auto* tw = static_cast<W*>(target);
            QPalette p = tw->palette();
            p.setColor(QPalette::WindowText, c);
            p.setColor(QPalette::Text, c);
            tw->setPalette(p);
        };
    }
}
#endif //LIFERHYTHM_WIDGETFACTORY_H
