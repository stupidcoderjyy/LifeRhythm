//
// Created by JYY on 24-10-18.
//

#ifndef OPENRANGE_H
#define OPENRANGE_H
#include <QVector>
#include "Plugin.h"
#include "Pair.h"

class CORE_API OpenRange {
private:
    QVector<int> pairsLeft;
    QVector<int> pairsRight;
    int offset;
public:
    static OpenRange fromString(const QString& expr);
    OpenRange(int l, int r);
    OpenRange();
    OpenRange(const OpenRange& o);
    OpenRange(OpenRange&& o) noexcept;
    inline OpenRange &operator=(OpenRange &&other) noexcept;
    inline OpenRange &operator=(const OpenRange &other);
    void move(int offset);
    int leftBorder();
    int rightBorder();
    void clear();
    void unionWith(int left, int right);
    void unionWith(int val);
    bool isEmpty() const;
    QString toString() const;
    OpenRange intersectWith(const OpenRange& other) const;
    OpenRange unionWith(OpenRange other);
    bool contains(const OpenRange& range) const;
    bool contains(int left, int right);
    bool contains(int val);
    void forEach(const std::function<void(int, int)> &action);
    int size() const;
    Pair<int, int> rangeAt(int pos);
    int lengthAt(int pos);
private:
    void removeIntervals(int start, int end);
    int findLastSmaller(int targetL) const;
    int findFirstLarger(int targetR) const;
    inline bool containsRange(int pos, int left, int right);
    inline void appendPair(int left, int right);
    inline void insertPair(int pos, int left, int right);
    inline void setPair(int pos, int left, int right);
    inline int actualLeft(int pos) const;
    inline int actualRight(int pos) const;
};

inline OpenRange & OpenRange::operator=(OpenRange &&other) noexcept {
    if (this != &other) {
        pairsLeft = std::move(other.pairsLeft);
        pairsRight = std::move(other.pairsRight);
        offset = other.offset;
    }
    return *this;
}

inline OpenRange & OpenRange::operator=(const OpenRange &other) {
    if (this != &other) {
        pairsLeft = other.pairsLeft;
        pairsRight = other.pairsRight;
        offset = other.offset;
    }
    return *this;
}

inline bool OpenRange::containsRange(int pos, int left, int right) {
    return left >= pairsLeft[pos] && right <= pairsRight[pos];
}

inline void OpenRange::appendPair(int left, int right) {
    if (isEmpty()) {
        pairsLeft[0] = left;
        pairsRight[0] = right;
    } else {
        pairsLeft << left;
        pairsRight << right;
    }
}

inline void OpenRange::insertPair(int pos, int left, int right) {
    pairsLeft.insert(pos, left);
    pairsRight.insert(pos, right);
}

inline void OpenRange::setPair(int pos, int left, int right) {
    pairsLeft[pos] = left;
    pairsRight[pos] = right;
}

inline int OpenRange::actualLeft(int pos) const {
    return pairsLeft[pos] + offset;
}

inline int OpenRange::actualRight(int pos) const {
    return pairsRight[pos] + offset;
}
#endif //OPENRANGE_H
