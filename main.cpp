
#include "Error.h"
#include "ListWidget.h"
#include "RcManagers.h"
#include <QTextCodec>
#include <QTimer>
#include <QDebug>
#include "LifeRhythm.h"
#include "TextLabel.h"
#include "models/WidgetData.h"
#include "TreeWidget.h"

class TestData : public TreeWidgetData {
public:
    int level;
    QString str;
    explicit TestData(QString str): TreeWidgetData(), str(std::move(str)), level(0){};

    void setParent(TreeWidgetData *parent) override {
        TreeWidgetData::setParent(parent);
        level = parent->cast<TestData>()->level + 1;
    }
};

class TestItem : public TreeItem {
private:
    TextLabel* label{};
public:
    explicit TestItem(QWidget* parent = nullptr): TreeItem(parent){
    };
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override {
        handlers << [](QWidget* target) {
            auto* item = static_cast<TestItem*>(target);
            item->label = item->getPointer<TextLabel>("label");
        };
    }

    void setFolded(bool f) override {
        TreeItem::setFolded(f);
        setState(!f);
    }

    void syncDataToWidget() override {
        TreeItem::syncDataToWidget();
        auto* d = data->cast<TestData>();
        setState(!d->isFolded());
        label->setText(QString("  ").repeated(d->level) + d->str);
    }

    void clearWidget() override {
        label->setText("");
        setState(0);
    }
};

class TestListWidget : public TreeWidget{
protected:
    ListItem *createRowItem() override {
        static WidgetFactory* loader = WidgetFactoryStorage::get("test:item");
        return static_cast<ListItem*>(loader->apply());
    }
};

USING_LR

int main(int argc, char *argv[]) {
    LifeRhythm lr(argc, argv);
    auto cfg = lr.getConfig();
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onMainInit([](){
        regClazz(WidgetFactoryStorage::get("test:item"), TestItem);
    });
    lr.onPostInit([](){
        auto* list = new TestListWidget;
        auto* model = new TreeModel("", "test.dat");
        for (int i = 0 ; i < 40 ; i ++) {
            *model << new TestData(QString::number(i));
        }
        for (int i = 0 ; i < 5 ; i ++) {
            auto name = QString((char)('a' + i));
            auto* p = new TestData(name);
            model->at(0)->cast<TestData>()->addChildren(p);
            model->at(1)->cast<TestData>()->addChildren(p);
        }
        list->setFixedSize(400, 400);
        list->setRowHeight(40);
        list->setMinAreaRowCount(10);
        list->setModel(model);
        list->show();
    }, Qt::QueuedConnection);
    return lr.launch();
}
