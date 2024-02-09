//
// Created by stupid_coder_jyy on 2024/1/14.
//

#ifndef LIFERHYTHM_WIDGETFACTORY_H
#define LIFERHYTHM_WIDGETFACTORY_H

#include <QMap>
#include <QWidget>
#include "StandardWidget.h"
#include "Identifier.h"

#define regClazz(factory, T) factory->registerStdWidget(#T, [](QWidget* p){ return new T(p); }, new T(nullptr))

class NBT;

class WidgetFactory{
    friend class WidgetFactoryStorage;
public:
    typedef std::function<QWidget*(QWidget* parent)> Supplier;
    typedef StandardWidget::Handlers Handlers;
private:
    enum State{
        Empty,
        Parsing,
        Ready,
        Broken
    };
    QMap<QString, Supplier>* customSuppliers;
    QMap<QString, StandardWidget*>* customEmptyInstances;
    QMap<QString, WidgetFactory*> childFactories{};
    QMap<int, Handlers> stateResponders{};
    Handlers globalResponders{};
    WidgetFactory* parentFactory;
    static QMap<QString, Supplier> stdSuppliers;
    static QMap<QString, StandardWidget*> stdEmptyInstances;
    Handlers handlers{};
    QWidget* workingWidget{};
    StandardWidget* stdType{};
    QString nbtPath{};
    NBT* source{};
    State state = Empty;
    QString id;

public:
    void setSource(NBT* nbt);
    void parse() noexcept;
    void registerStdWidget(const QString& type, const Supplier& supplier, StandardWidget* instance);
    QWidget* apply(QWidget* parent = nullptr, QWidget* target = nullptr) noexcept;
    static void init();
    static WidgetFactory* fromResource(const Identifier& loc);
    static WidgetFactory* fromNbt(const QString& id, NBT* nbt);
    virtual ~WidgetFactory();
private:
    explicit WidgetFactory(QString id);
    explicit WidgetFactory(WidgetFactory* parent, const QString& id, NBT* nbt);
    static QRegExp regexId;
    StandardWidget* parseWidgetType(NBT* nbt);
    void parseChildren(NBT* nbt);
    WidgetFactory* findFactory(NBT* nbt, const QString& path);
    void parseLayout(NBT* target);
    void parseMargins(NBT* nbt);
    void parseStates(NBT* nbt);
    void parseSingleState(Handlers& op, NBT* stateTag);
    void parsePointer(NBT* nbt);
    static void parseQss(Handlers& target, NBT* nbt);
    static void parseSize(Handlers& op, NBT* nbt);
    QWidget* createWidget(const QString& type, QWidget* parent = nullptr);
};
#endif //LIFERHYTHM_WIDGETFACTORY_H
