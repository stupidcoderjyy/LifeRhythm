//
// Created by JYY on 24-12-24.
//

#ifndef IMAGERCLOADER_H
#define IMAGERCLOADER_H
#include <RcLoader.h>

BEGIN_LR

class CORE_API ImageRcLoader final : public RcLoader<QPixmap> {
public:
    QPixmap *load(const Identifier &loc, const QString &absolutePath) override;
SINGLETON_HEADER(ImageRcLoader)
};

END_LR

#endif //IMAGERCLOADER_H
