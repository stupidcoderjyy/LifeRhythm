//
// Created by stupid_coder_jyy on 2024/2/26.
//

#include "ListModel.h"

#define super ListDataLoader<WidgetData>

ListModel::ListModel(const QString &path, const QString &fileName): super(path, fileName) {
}

int ListModel::length() const {
    return es.length();
}

WidgetData *ListModel::at(int idx) noexcept {
    if (idx < 0 || idx >= es.length()) {
        return nullptr;
    }
    return es.at(idx);
}

void ListModel::append(WidgetData *data) {
    int old = es.length();
    es.append(data);
    emit sigDataChanged(old, old);
}

void ListModel::insert(int idx, WidgetData *data) {
    es.insert(idx, data);
    emit sigDataChanged(idx, es.length() - 1);
}

WidgetData *ListModel::remove(int idx) {
    auto* d = es.at(idx);
    es.remove(idx);
    emit sigDataChanged(idx, idx < es.length() ? es.length() - 1 : idx);
    return d;
}
