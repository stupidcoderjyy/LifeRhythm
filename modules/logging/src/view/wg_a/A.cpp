//
// Created by stupid_coder_jyy on 2024/3/2.
//

#include "A.h"
#include "Period.h"
#include "A1.h"
#include "DataA1.h"

USING_NAMESPACE(lr::log)

A::A(QWidget *parent): TabWidget(parent) {
}

void A::onTabCreated() {
    bar = getPointer<A0>("timeBar");
    auto* d = new NestedListData;
    d->makeRow();
    d->append(0, new Period(4 * 60, 5 * 60, "aaaaa"));
    d->append(0, new Period(5 * 60, 8 * 60, "bbbbbb"));
    bar->setData(d);
    d->markAll();

    auto* tree = new TreeData;
    auto* n0 = new TreeNode(new DataA1(new Usage(new UsageType(Styles::CYAN_0->color, "type"), "工流 P1-3", 120), true));
    auto* n00 = new TreeNode(new DataA1(new Usage(new UsageType(Styles::CYAN_0->color, "type"), "测试1", 120), false));
    auto* n01 = new TreeNode(new DataA1(new Usage(new UsageType(Styles::BLUE_0->color, "type"), "测试2", 15), true));
    n0->addChildren(tree, n00);
    n0->addChildren(tree, n01);
    tree->addNode(n0);
    auto* a1 = getPointer<A1>("a1");
    a1->setData(tree);
    tree->foldNode(0, false);
}
