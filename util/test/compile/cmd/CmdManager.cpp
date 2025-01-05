//
// Created by JYY on 25-1-5.
//

#include "CmdManager.h"

#include "Helpers.h"

USING_NP(lr::cmd)

Command::Command(Identifier id): id(std::move(id)) {
}

int Command::exec(const QStringList &args) {
    return 0;
}

CommandContainer::CommandContainer(QString ns): ns(std::move(ns)) {
}

CommandContainer::~CommandContainer() {
    DELETE_MAP(commands);
    DELETE_MAP(children);
}

void CmdManager::registerCommand(Command* command) {
    auto id = command->getId();
    auto rootNs = id.getNamespace();
    auto root = roots.value(rootNs);
    if (!root) {
        root = new CommandContainer(rootNs);
        roots.insert(rootNs, root);
    }
    auto paths = id.getPath().split('/');
    auto node = root;
    for (int i = 0 ; i < paths.length() - 1 ; i ++) {
        const auto& childPath = paths[i];
        auto child = node->getChild(childPath);
        if (!child) {
            child = new CommandContainer(childPath);
            roots.insert(childPath, child);
        }
        node = child;
    }
    delete node->getCommand(paths.last());
    node->commands.insert(paths.last(), command);
}

CmdManager::~CmdManager() {
    DELETE_MAP(roots)
}
