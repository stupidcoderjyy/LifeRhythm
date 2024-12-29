//
// Created by JYY on 24-12-20.
//

#ifndef NAMESPACES_H
#define NAMESPACES_H

#define BEGIN_NP(x) namespace x {
#define END_NP }
#define USING_NP(x) using namespace x;

#define BEGIN_LR BEGIN_NP(lr)
#define END_LR END_NP
#define USING_LR USING_NP(lr)

#endif //NAMESPACES_H
