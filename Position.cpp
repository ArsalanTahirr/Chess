#include <iostream>
#include <utility>
#include "Position.h"

Position::Position(): X(-1), Y(-1) {}

Position::Position(const int X, const int Y): X(X), Y(Y) {}

Position::~Position() {}

void Position::setCoordinate(const int X, const int Y) {
    this->X = X;
    this->Y = Y;
}

bool Position::operator== (const Position& obj) const {
    return X == obj.X && Y == obj.Y;
}

bool Position::operator!= (const Position& obj) const {
    return !(*this == obj);
}