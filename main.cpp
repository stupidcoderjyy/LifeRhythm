

#include <QLabel>
#include <utility>
#include "SelectableListWidget.h"
#include "OptionsBox.h"
#include "Namespaces.h"
#include "LifeRhythm.h"

class ColorData : public WidgetData {
public:
    QString color;
    explicit ColorData(QString color): WidgetData(), color(std::move(color)) {}
};

class ColorItem : public SelectableListItem {
private:
    QLabel* label;
public:
    explicit ColorItem(QWidget* parent = nullptr): SelectableListItem(parent){
        auto* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0,0,0,0);
        setLayout(layout);
        label = new QLabel(this);
        layout->addWidget(label);
        label->setFont(Styles::FONT_MAIN);
    }
    void syncDataToWidget() override {
        QString c = wData->cast<ColorData>()->color;
        label->setText(c);
        label->setStyleSheet(bg(c) + qss("color", Styles::GRAY_TEXT_0));
    }
};

class ColorListWidget : public SelectableListWidget {
public:
    explicit ColorListWidget(QWidget* parent = nullptr): SelectableListWidget(parent) {
    }
protected:
    SelectableListItem *createRowItem() override {
        return new ColorItem;
    }
};

class ColorsBox : public OptionsBox {
public:
    explicit ColorsBox(QWidget* parent = nullptr): OptionsBox(parent) {
    }
protected:
    void initMenu(OptionsMenu* menu) override {
        menu->setFixedHeight(200);
        auto* layout = new QVBoxLayout;
        layout->setContentsMargins(1,1,1,1);
        menu->setLayout(layout);
        auto* list = new ColorListWidget(menu);
        auto* model = new SelectableListData();
        for (auto it = Styles::colors.begin() ; it != Styles::colors.end() ; it++) {
            model->append(new ColorData(it.value().name(QColor::NameFormat::HexRgb)));
        }
        connect(model, &SelectableListData::sigDataSelected, this, [model, menu](int pre, int cur){
            emit menu->sigSelectOption(model->at(cur));
        });
        list->setData(model);
        list->setRowHeight(30);
        layout->addWidget(list);
    }
    void fillOption(WidgetData *data) override {
        optionEditor->setText(data->cast<ColorData>()->color);
    }
};

USING_LR

int main(int argc, char *argv[]) {
    LifeRhythm lr(argc, argv);
    auto cfg = lr.getConfig();
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onMainInit([](){

    });
    lr.onPostInit([](){
        auto* root = new QWidget;
        root->setObjectName("r");
        root->setStyleSheet(qss_t("r", bg(Styles::BLACK)));
        auto* layout = new QVBoxLayout(root);
        root->setLayout(layout);
        layout->addWidget(new ColorsBox(root));
        root->show();
    }, Qt::QueuedConnection);
    return lr.launch();
}
