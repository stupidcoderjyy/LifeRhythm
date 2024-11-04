//
// Created by JYY on 24-10-18.
//

#include "OpenRange.h"

#include <qregularexpression.h>
#include <QStringBuilder>

#include "StringInput.h"
#include "openrange/SyntaxAnalyzer.h"

OpenRange OpenRange::fromString(const QString &expr) {
    return lr::openrange::SyntaxAnalyzer::parse(expr);
}

OpenRange::OpenRange(int l, int r): offset(0) {
    pairsLeft.reserve(4);
    pairsRight.reserve(4);
    pairsLeft << l;
    pairsRight << r;
}

OpenRange::OpenRange(): OpenRange(0, 0) {
}

OpenRange::OpenRange(const OpenRange &o) {
    pairsLeft = o.pairsLeft;
    pairsRight = o.pairsRight;
    offset = o.offset;
}

OpenRange::OpenRange(OpenRange &&o) noexcept {
    pairsLeft = std::move(o.pairsLeft);
    pairsRight = std::move(o.pairsRight);
    offset = o.offset;
}

void OpenRange::move(int offset) {
    this->offset += offset;
}

int OpenRange::leftBorder() {
    return isEmpty() ? INT32_MIN : pairsLeft[0] + offset;
}

int OpenRange::rightBorder() {
    return isEmpty() ? INT32_MAX : pairsRight[0] + offset;
}

void OpenRange::clear() {
    offset = 0;
    pairsLeft.clear();
    pairsRight.clear();
}

void OpenRange::unionWith(int left, int right) {
    left -= offset;
    right -= offset;
    if (right - left < 2) {
        return;
    }
    if (isEmpty()) {
        appendPair(left, right);
        return;
    }
    int lPos = findLastSmaller(left);
    int rPos = findFirstLarger(right);
    if (pairsLeft.size() == 1) {
        auto left0 = pairsLeft.at(0);
        auto right0 = pairsRight.at(0);
        if (right <= left0) {
            insertPair(0, left, right);
        } else if (left >= right0) {
            appendPair(left, right);
        } else {
            setPair(0, qMin(left0, left), qMax(right0, right));
        }
        return;
    }
    int resetId;
    if (lPos + 1 > pairsLeft.size() - 1) {
        resetId = -1;
    } else {
        if (lPos < 0) {
            if (rPos == 0) {
                insertPair(0, left, right);
                return;
            }
        } else if (rPos >= 0 && rPos - lPos == 1) {
            insertPair(lPos + 1, left, right);
            return;
        }
        resetId = lPos < 0 ? 0 : lPos + 1;
    }

    int removeR = rPos < 0 ? pairsLeft.size() - 1 : rPos - 1;
    if (resetId == -1) {
        appendPair(left, right);
        resetId = lPos + 1;
    }
    int min = qMin(pairsLeft.at(resetId), left);
    int max = qMax(pairsRight.at(removeR), right);
    removeIntervals(resetId + 1, removeR);
    setPair(resetId ,min, max);
}

void OpenRange::unionWith(int val) {
    unionWith(val - 1, val + 1);
}

bool OpenRange::isEmpty() const {
    if (pairsLeft.size() > 1) {
        return false;
    }
    return pairsRight[0] - pairsLeft[0] < 2;
}

QString OpenRange::toString() const {
    if (isEmpty()) {
        return "()";
    }
    QString sb = '(' % QString::number(actualLeft(0)) % ',' % QString::number(actualRight(0)) % ')';
    for (int i = 1; i < pairsLeft.size() ; i ++) {
        sb = sb % "U(" % QString::number(actualLeft(i)) % ',' % QString::number(actualRight(i)) % ')';
    }
    return sb;
}

OpenRange OpenRange::intersectWith(const OpenRange &other) const {
    OpenRange result;
    if (other.isEmpty() || this->isEmpty()) {
        return result;
    }
    int posA = 0;
    int posB = 0;
    int sizeA = other.pairsLeft.size();
    int sizeB = this->pairsLeft.size();
    while (posA < sizeA && posB < sizeB) {
        int leftA = other.actualLeft(posA), rightA = other.actualRight(posA);
        int leftB = this->actualLeft(posB), rightB = this->actualRight(posB);

        //无交集的情况
        if (rightB <= leftA) {
            //pairThis在左侧
            posB++;
            continue;
        }
        if (rightA <= leftB) {
            //pairThis在右侧
            posA++;
            continue;
        }
        //有交集
        int leftInters = qMax(leftB, leftA);
        if (int rightInters = qMin(rightB, rightA); leftInters != rightInters) {
            result.appendPair(leftInters, rightInters);
        }
        if (rightB <= rightA) {
            //pairThis偏左
            posB++;
        } else {
            //pairThis偏右
            posA++;
        }
    }
    return result;
}

OpenRange OpenRange::unionWith(OpenRange other) {
    if (other.isEmpty()) {
        return *this;
    }
    if (isEmpty()) {
        return other;
    }
    OpenRange result;
    int posA = 0;
    int posB = 0;
    int sizeA = other.pairsLeft.size();
    int sizeB = pairsLeft.size();
    bool resetA = true, resetB = true;
    int leftA = 0,rightA = 0;
    int leftB = 0,rightB = 0;
    while (true) {
        if (resetA) {
            if (posA >= sizeA) {
                result.appendPair(leftB, rightB);
                break;
            }
            leftA = other.actualLeft(posA);
            rightA = other.actualRight(posA);
            resetA = false;
        }
        if (resetB) {
            if (posB >= sizeB) {
                result.appendPair(leftA, rightA);
                break;
            }
            leftB = actualLeft(posB);
            rightB = actualRight(posB);
            resetB = false;
        }
        //无交集的情况
        if (rightB <= leftA) {
            //pairB在左侧
            result.appendPair(leftB, rightB);
            resetB = true;
            posB++;
            continue;
        }
        if (rightA <= leftB) {
            //pairB在右侧
            result.appendPair(leftA, rightA);
            resetA = true;
            posA++;
            continue;
        }
        //有交集
        int leftUnion = qMin(leftB, leftA);
        if (rightA > rightB) {
            //pairA偏右
            leftA = leftUnion;
            resetB = true;
            posB++;
        } else {
            leftB = leftUnion;
            resetA = true;
            posA++;
        }
    }
    posA++;
    posB++;
    //把剩余部分直接加进去
    while (posA < sizeA) {
        result.pairsLeft << other.actualLeft(posA);
        result.pairsRight << other.actualRight(posA);
        posA++;
    }
    while (posB < sizeB) {
        result.pairsLeft << actualLeft(posB);
        result.pairsRight << actualRight(posB);
        posB++;
    }
    return result;
}

bool OpenRange::contains(const OpenRange &range) const {
    if (range.isEmpty()) {
        return true;
    }
    int posThis = 0, posOther = 0;
    int sizeThis = pairsLeft.size(), sizeOther = range.pairsLeft.size();
    while (true) {
        if (posOther == sizeOther) {
            return true;
        }
        if (posThis == sizeThis) {
            return false;
        }
        int leftThis = actualLeft(posThis), rightThis = actualRight(posThis);
        int leftOther = range.actualLeft(posOther), rightOther = range.actualRight(posOther);
        //无交集
        if (leftOther >= rightThis) {
            posThis++;
            continue;
        }
        if (leftThis >= rightOther) {
            return false;
        }
        //有交集
        if (leftOther >= leftThis && rightOther <= rightThis) {
            //在区间内
            posOther++;
            posThis++;
            continue;
        }
        //不符合
        return false;
    }
}

bool OpenRange::contains(int left, int right) {
    left -= offset;
    right -= offset;
    if (isEmpty()) {
        return false;
    }
    if (pairsLeft.size() == 1) {
        return containsRange(0, left, right);
    }
    int posR = findFirstLarger(right);
    if (posR == 0) {
        return containsRange(0, left, right);
    }
    return containsRange(posR < 0 ? pairsLeft.size() - 1 : posR - 1, left, right);
}

int OpenRange::indexOf(int left, int right) {
    left -= offset;
    right -= offset;
    if (isEmpty()) {
        return false;
    }
    if (pairsLeft.size() == 1 && containsRange(0, left, right)) {
        return 0;
    }
    int posR = findFirstLarger(right);
    if (posR == 0 && containsRange(0, left, right)) {
        return 0;
    }
    int pos = posR < 0 ? pairsLeft.size() - 1 : posR - 1;
    return containsRange(pos, left, right) ? pos : -1;
}

void OpenRange::forEach(const std::function<void(int, int)> &action) {
    if (isEmpty()) {
        return;
    }
    for (int i = 0 ; i < pairsLeft.size() ; i ++) {
        action(pairsLeft[i], pairsRight[i]);
    }
}

int OpenRange::size() const {
    if (isEmpty()) {
        return 0;
    }
    return pairsLeft.size();
}

Pair<int, int> OpenRange::rangeAt(int pos) {
    return {pairsLeft[pos], pairsRight[pos]};
}

int OpenRange::lengthAt(int pos) {
    return pairsRight[pos] - pairsLeft[pos] - 1;
}

void OpenRange::removeIntervals(int start, int end) {
    start = qMax(start, 0);
    end = qMin(end, pairsLeft.size() - 1);
    if (start < end) {
        pairsLeft.remove(start, end - start + 1);
        pairsRight.remove(start, end - start + 1);
    } else if (start == end) {
        pairsLeft.remove(start);
        pairsRight.remove(start);
    }
}

int OpenRange::findLastSmaller(int targetL) const {
    if (pairsLeft.size() == 1) {
        if (pairsRight.at(0) < targetL) {
            return 0;
        }
        return -1;
    }
    int left = 0, right = pairsLeft.size() - 2;
    //把intervals.at(middle)右侧的空当视为一个区间
    while (left <= right) {
        int middle = (left + right) / 2;
        if (pairsRight.at(middle) > targetL){
            right = middle - 1; //target在左侧
        } else if (pairsRight.at(middle + 1) <= targetL) {
            left = middle + 1;//target在右侧
        } else {
            return middle;
        }
    }
    if (left >= pairsLeft.size() - 1) {
        return pairsLeft.size() - 1;
    }
    return -1;
}

int OpenRange::findFirstLarger(int targetR) const {
    if (pairsLeft.size() == 1) {
        if (pairsLeft.at(0) > targetR) {
            return 0;
        }
        return -1;
    }
    int left = 1, right = pairsLeft.size() - 1;
    //把intervals.at(middle)左侧的空当视为一个区间
    while (left <= right) {
        int middle = (left + right) / 2;
        if (pairsLeft.at(middle - 1) >= targetR) {
            right = middle - 1; //target在左侧
        } else if (pairsLeft.at(middle) < targetR) {
            left = middle + 1; //target在右侧
        } else {
            return middle;
        }
    }
    if (right <= 0) {
        return 0;
    }
    return -1;
}
