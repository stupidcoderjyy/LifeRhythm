
#include "QDebug"
#include <QApplication>
#include "Error.h"
#include "CompileError.h"
#include "PrintErrorHandler.h"
#include "DataLoader.h"
#include "ListWidget.h"
#include "RcManagers.h"
#include <QTextCodec>
#include <QTimer>
#include <QDebug>
#include "LifeRhythm.h"
#include "TextLabel.h"
#include "WidgetData.h"

class TestData : public WidgetData {
public:
    QString str;
    explicit TestData(QString str): WidgetData(), str(std::move(str)){};
};

class TestItem : public ListItem {
private:
    TextLabel* label{};
public:
    explicit TestItem(QWidget* parent = nullptr): ListItem(parent){
    };
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override {
        handlers << [](QWidget* target) {
            auto* item = static_cast<TestItem*>(target);
            item->label = item->getPointer<TextLabel>("label");
        };
    }
    void syncDataToWidget() override {
        label->setText(data->cast<TestData>()->str);
        setState(dataIdx & 1);
    }
    void clearWidget() override {
        label->setText("");
        setState(0);
    }
};

class TestListWidget : public ListWidget{
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
        auto* model = new ListedDataLoader<WidgetData>("", "test.dat");
        for (int i = 0 ; i < 40 ; i ++) {
            *model << new TestData(QString::number(i));
        }
        list->setFixedSize(400, 400);
        list->setRowHeight(40);
        list->setMinAreaRowCount(10);
        list->setModel(model);
        list->setObjectName("list");
        list->show();
        QTimer::singleShot(3000, [list](){
            list->setFixedHeight(440);
        });
    }, Qt::QueuedConnection);
    return lr.launch();
}
