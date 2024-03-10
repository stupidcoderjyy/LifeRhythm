//
// Created by stupid_coder_jyy on 2024/3/2.
//

#include "PeriodType.h"
#include <utility>

PeriodType::PeriodType(QColor color, QString name): color(std::move(color)), name(std::move(name)) {
}

PeriodType::PeriodType(const PeriodType &o): color(o.color), name(o.name) {
}

PeriodType::PeriodType(): color(), name() {
}

void PeriodType::toBytes(IByteWriter *writer) {
    TreeNode::toBytes(writer);
    writer->writeInt(color.value());
    writer->writeString(name);
}

void PeriodType::fromBytes(IByteReader *reader) {
    TreeNode::fromBytes(reader);
    color = reader->readInt();
    name = reader->readString();
}

PeriodTypeTree::PeriodTypeTree(): SelectableTreeData() {
}

TreeNode *PeriodTypeTree::readElement(IByteReader *reader) {
    return new PeriodType;
}

#include <QDebug>

void PeriodTypeTree::onNodeExpanded(TreeNode *node) {
    qDebug() << node->cast<PeriodType>()->name;
}

PeriodTypeLoader::PeriodTypeLoader(QString destPath): DataLoader(std::move(destPath)) {
}

WidgetData *PeriodTypeLoader::createData() noexcept {
    return new PeriodTypeTree;
}
