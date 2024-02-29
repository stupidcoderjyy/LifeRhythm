//
// Created by stupid_coder_jyy on 2024/2/26.
//

#include "TreeModel.h"

TreeWidgetData::TreeWidgetData():WidgetData(), parent(), children(), folded(true) {
}

void TreeWidgetData::addChildren(TreeWidgetData *c) {
    children << c;
    c->setParent(this);
}

void TreeWidgetData::setParent(TreeWidgetData *p) {
    parent = p;
}

bool TreeWidgetData::isFolded() const {
    return folded;
}

#define super ListDataLoader<TreeWidgetData>

TreeModel::TreeModel(const QString &path, const QString &fileName): super(path, fileName) {
}

int TreeModel::length() const {
    return es.length();
}

WidgetData *TreeModel::at(int idx) noexcept {
    if (idx < 0 || idx >= es.length()) {
        return nullptr;
    }
    return es.at(idx);
}

void TreeModel::append(WidgetData *data) {
    int old = es.length();
    es.append(data->cast<TreeWidgetData>());
    emit sigDataChanged(old, old);
}

void TreeModel::insert(int idx, WidgetData *data) {
    es.insert(idx, data->cast<TreeWidgetData>());
    emit sigDataChanged(idx, es.length() - 1);
}

WidgetData *TreeModel::remove(int idx) {
    auto* d = es.at(idx);
    es.remove(idx);
    emit sigDataChanged(idx, idx < es.length() ? es.length() - 1 : idx);
    return d;
}

void TreeModel::onFolded(int idx, bool folded) {
    auto& es = super::es;
    auto* item = es.at(idx);
    int count = item->children.count();
    if (!count) {
        return;
    }
    idx++;
    if (!folded) {
        es.insert(idx, count, nullptr);
        memcpy(es.data() + idx, item->children.data(), count << 3);
    } else {
        es.remove(idx, count);
    }
    emit sigDataChanged(idx - 1, qMax(idx, es.length() - 1));
}
