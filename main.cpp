
#include "Error.h"
#include "ListWidget.h"
#include "RcManagers.h"
#include <QTextCodec>
#include <QTimer>
#include <QDebug>
#include "LifeRhythm.h"
#include "TextLabel.h"
#include "models/WidgetData.h"
#include "SelectableListWidget.h"

class TestData : public WidgetData {
public:
    QString str;
    explicit TestData(QString str): WidgetData(), str(std::move(str)){
    };
};

class TestItem : public SelectableListItem {
private:
    TextLabel* label{};
public:
    explicit TestItem(QWidget* parent = nullptr): SelectableListItem(parent){
    };
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override {
        handlers << [](QWidget* target) {
            auto* item = static_cast<TestItem*>(target);
            item->label = item->getPointer<TextLabel>("label");
        };
    }

    void syncDataToWidget() override {
        SelectableListItem::syncDataToWidget();
        auto* d = data->cast<TestData>();
        label->setText(d->str);
    }

    void clearWidget() override {
        label->setText("");
        setState(0);
    }

    void updateItemAfterSelecting(bool selected) override {
        setState(selected);
    }
};

class TestListWidget : public SelectableListWidget{
protected:
    SelectableListItem *createRowItem() override {
        static WidgetFactory* loader = WidgetFactoryStorage::get("test:item");
        auto* item = static_cast<TestItem*>(loader->apply());
        item->setModel(static_cast<SelectableListModel*>(model));
        return item;
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
        auto* model = new SelectableListModel("", "test.dat");
        for (int i = 0 ; i < 40 ; i ++) {
            *model << new TestData(QString::number(i));
        }
        list->setFixedSize(400, 400);
        list->setRowHeight(40);
        list->setMinAreaRowCount(10);
        list->setModel(model);
        list->show();
        QTimer::singleShot(3000, [model](){
            model->insert(2, new TestData("dwahidhwad"));
        });
        QObject::connect(model, &SelectableListModel::sigDataSelected, [model](int pre, int cur){
            qDebug() << model->at(cur)->cast<TestData>()->str;
        });
    }, Qt::QueuedConnection);
    return lr.launch();
}
