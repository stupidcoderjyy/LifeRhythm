//
// Created by stupid_coder_jyy on 2024/3/2.
//

#ifndef LIFERHYTHM_PERIODTYPE_H
#define LIFERHYTHM_PERIODTYPE_H

#include <QColor>
#include "TreeData.h"
#include "DataLoader.h"

class PeriodType : public TreeNode {
    friend class PeriodTypeTree;
private:
    QColor color;
    QString name;
public:
    PeriodType(QColor color, QString name);
    PeriodType(const PeriodType& o);
    PeriodType();
    void toBytes(IByteWriter *writer) override;
    void fromBytes(IByteReader *reader) override;
};

class PeriodTypeTree : public TreeData {
public:
    PeriodTypeTree();
protected:
    TreeNode *readElement(IByteReader *reader) override;
    void expandNode(TreeNode *node) override;
};

class PeriodTypeLoader : public DataLoader {
public:
    explicit PeriodTypeLoader(QString destPath);
protected:
    WidgetData *emptyElement() noexcept override;
};


#endif //LIFERHYTHM_PERIODTYPE_H
