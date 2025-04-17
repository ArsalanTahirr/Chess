#include <iostream>
#include <cctype>
#include "Piece.h"
#include "Position.h"
#include "Board.h"
#include "Enums.h"

Piece::Piece(Color color, const char character, const Position& position) :
color(color), position(position), isCaptured(false) {
    this->character = (color == Color::Black ? tolower(character): toupper(character));
    symbolForBoard = getPieceSymbol(this->character);
}

Piece::~Piece() {}

Color Piece::getColor() const { return color; }

bool Piece::getisCaptured() const { return isCaptured; }

void Piece::setisCaptured(bool captured) { isCaptured = captured; }

Position Piece::getPosition() const { return position; }

void Piece::setPosition(int row, int col) { position.setCoordinate(row, col); }

bool Piece::canMove(const Position& to, Board* board) const {
    std::vector<Position> ValidMoves = getValidMoves(board);
    bool flag = false;
    for(Position& position: ValidMoves) {
        if(position == to) {
            flag = true;
            break;
        }
    }
    return flag;
}

std::string Piece::getPieceSymbol(char piece) const {
    switch (piece) {
        case 'K': return "♔";
        case 'Q': return "♕";
        case 'R': return "♖";
        case 'B': return "♗";
        case 'N': return "♘";
        case 'P': return "♙";
        case 'k': return "♚";
        case 'q': return "♛";
        case 'r': return "♜";
        case 'b': return "♝";
        case 'n': return "♞";
        case 'p': return "♟";
        default:  return " ";
    }
}