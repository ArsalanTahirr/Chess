#include <iostream>
#include <utility>
#include "Position.h"

Position::Position(int row, int column): row(row), column(column) {}

Position::~Position() {}

int Position::getRow() const { return row; }

int Position::getColumn() const { return column; }

std::pair<int,int> Position::getCoordinate() const { return std::make_pair(row, column); }

void Position::setCoordinate(int row, int column) {
    this->row = row;
    this->column = column;
}

bool Position::operator== (const Position& obj) {
    return row == obj.row && column == obj.column;
}