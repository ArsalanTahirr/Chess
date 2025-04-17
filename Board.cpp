#include <iostream>
#include "Board.h"
#include "Piece.h"
#include "GlobalVariables.h"

Board::Board(std::vector<std::vector<Square>>& squares): squares(squares) {}

Board::~Board() {}

Piece* Board::getPiece(const Position& position) const { return squares.at(position.getRow()).at(position.getColumn()).getPiece(); }

void Board::placePiece(Piece *piece, const Position& position) { squares.at(position.getRow()).at(position.getColumn()).setPiece(piece); }

void Board::removePiece(const Position& position) { placePiece(nullptr, position); }

bool Board::isSquareOccupied(const Position& position) const { return !getPiece(position);}

std::vector<Piece*> Board::getAllPieces() const {
    std::vector<Piece*> pieces;
    for(int i = 0; i < BOARD_ROWS; i++) {
        for(int j = 0; j < BOARD_COLS; j++) {
            Piece* piece = getPiece(Position(i , j));
            if(piece != nullptr) {
                pieces.push_back(piece);
            }
        }
    }
    return pieces;
}