//
// Created by stupid_coder_jyy on 2024/3/2.
//

#include "Type.h"
#include <utility>

Type::Type(QString color, QString name): color(std::move(color)), name(std::move(name)) {
}

Type::Type(const Type &o): color(o.color), name(o.name) {
}
