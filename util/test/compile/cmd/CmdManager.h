//
// Created by JYY on 25-1-5.
//

#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H
#include <QMap>

#include "Identifier.h"
#include "Namespaces.h"

BEGIN_NP(lr::cmd)

class Command {
private:
    Identifier id;
public:
    explicit Command(Identifier id);
    virtual ~Command() = default;
    Identifier getId() const { return id; };
protected:
    virtual int exec(const QStringList& args);
};

class CommandContainer {
    friend class CmdManager;
private:
    QString ns;
    QMap<QString, Command*> commands;
    QMap<QString, CommandContainer*> children;
public:
    explicit CommandContainer(QString ns);
    ~CommandContainer();
    inline CommandContainer* getChild(const QString& ns) const;
    inline Command* getCommand(const QString& ns) const;
    inline QString getNamespace() const;
};

inline CommandContainer* CommandContainer::getChild(const QString& ns) const { return children.value(ns); }
inline Command* CommandContainer::getCommand(const QString& ns) const { return commands.value(ns); }
inline QString CommandContainer::getNamespace() const { return ns; }

class CmdManager {
private:
    QMap<QString, CommandContainer*> roots;
public:
    void registerCommand(Command* command);
    CommandContainer* getRoot(const QString& rootNamespace) const { return roots.value(rootNamespace); }
    ~CmdManager();
};

END_NP

#endif //COMMANDMANAGER_H
