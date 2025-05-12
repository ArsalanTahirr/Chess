#include <iostream>
#include "../../include/core/Square.h"

Square::Square(): position(Position(0, 0)), piece(nullptr), color(Color::White) {}

Square::Square(const Position& position, Piece *piece): position(position), piece(piece) {
    if((position.X + position.Y) % 2 == 0) color = Color::Black;
    else color = Color::White;
}

Square::~Square() {}

Piece* Square::getPiece() const { return piece; }

void Square::setPiece(Piece *piece) { this->piece = piece; }

bool Square::isEmpty() const { return !piece; } // if piece is nullptr than the square is empty

Position Square::getPosition() const { return position; }

Color Square::getColor() const { return color; } 

