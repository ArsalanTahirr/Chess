#include <iostream>
#include "Square.h"


Square::Square(const Position& position, Piece *piece): position(position), piece(piece) {
    if((position.getRow() + position.getColumn()) % 2 == 0) color = Color::Black;
    else color = Color::White;
}

Square::~Square() {}

Piece* Square::getPiece() const { return piece; }

void Square::setPiece(Piece *piece) { this->piece = piece; }

bool Square::isEmpty() const { return !piece; } // if piece is nullptr than the square is empty

Position Square::getPosition() const { return position; }

Color Square::getColor() const { return color; } 