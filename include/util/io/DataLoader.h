//
// Created by stupid_coder_jyy on 2024/2/22.
//

#ifndef LIFERHYTHM_DATALOADER_H
#define LIFERHYTHM_DATALOADER_H

#include "WidgetData.h"

class DataLoader : public QObject {
    Q_OBJECT
protected:
    QString destPath;   //存档的位置
    bool loaded{};
    WidgetData* wData;
public:
    DataLoader(const QString& savePath, const QString& childPath);
    virtual void load();
    virtual void unload();
    virtual void save();
signals:
    void sigLoaded();
    void sigUnloaded();
protected:
    virtual WidgetData* emptyElement() noexcept = 0;
    virtual void deleteElement(WidgetData* p) noexcept;
};

#endif //LIFERHYTHM_DATALOADER_H
