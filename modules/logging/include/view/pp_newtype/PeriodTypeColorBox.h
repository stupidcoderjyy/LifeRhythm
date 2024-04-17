//
// Created by stupid_coder_jyy on 2024/3/25.
//

#ifndef LIFERHYTHM_PERIODTYPECOLORBOX_H
#define LIFERHYTHM_PERIODTYPECOLORBOX_H

#include "AbstractOptionsBox.h"

class ColorIcon;
class HueSelector;

class PeriodTypeColorBox : public AbstractOptionsBox {
private:
    Color* iconData;
    ColorIcon* icon;
    HueSelector* hueSelector;
public:
    explicit PeriodTypeColorBox(QWidget* parent = nullptr);
    ~PeriodTypeColorBox() override;
protected:
    void initMenu(OptionsMenu *menu) override;
};

#endif //LIFERHYTHM_PERIODTYPECOLORBOX_H
