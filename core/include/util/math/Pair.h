//
// Created by JYY on 24-10-18.
//

#ifndef PAIR_H
#define PAIR_H

template<typename L, typename R>
struct CORE_API Pair {
    L l;
    R r;
    Pair copy() const {
        return {l, r};
    }
};



#endif //PAIR_H
