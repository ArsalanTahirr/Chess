#include <iostream>
#include "Pawn.h"
#include "GlobalVariables.h"

Pawn::Pawn(Color color, const char character, const Position& position):
Piece(color, character, position), hasMoved(false) {}

Pawn::~Pawn() {}

std::vector<Position> Pawn::getValidMoves(Board* board) const {
    std::vector<Position> validMoves;
    int row = position.getRow();
    int col = position.getColumn();

    int directions[4][2] = {
        {0, 1}, {0, 2} , // forward move and double move
        {1, 1}, {-1, 1} // capture moves (diagonal) 
    };
    
        int newRow1 = row + directions[0][0];
        int newCol1 = col + directions[0][1];
        int newRow2 = row + directions[1][0];
        int newCol2 = col + directions[1][1];
        int newRow3 = row + directions[2][0];
        int newCol3 = col + directions[2][1];
        int newRow4 = row + directions[3][0];
        int newCol4 = col + directions[3][1];

        if (newRow1 > 0 && newRow1 <= BOARD_ROWS &&
            newCol1 > 0 && newCol1 <= BOARD_COLS) {
            Position newPos(newRow1, newCol1);
            Piece* pieceAtNewPos = board->getPiece(newPos);
        
            if (pieceAtNewPos == nullptr) {
                validMoves.push_back(newPos);
            } 
        }
        if (newRow2 > 0 && newRow2 <= BOARD_ROWS &&
            newCol2 > 0 && newCol2 <= BOARD_COLS) {
            Position newPos(newRow2, newCol2);
            Piece* pieceAtNewPos = board->getPiece(newPos);

            if (pieceAtNewPos == nullptr && !hasMoved) {
                validMoves.push_back(newPos);
            } 
        }
        if (newRow3 > 0 && newRow3 <= BOARD_ROWS &&
            newCol3 > 0 && newCol3 <= BOARD_COLS) {
            Position newPos(newRow3, newCol3);
            Piece* pieceAtNewPos = board->getPiece(newPos);
        
            if (pieceAtNewPos != nullptr && pieceAtNewPos->getColor() != color) {
                validMoves.push_back(newPos);
            } 
        }
        if (newRow4 > 0 && newRow4 <= BOARD_ROWS &&
            newCol4 > 0 && newCol4 <= BOARD_COLS) {
            Position newPos(newRow4, newCol4);
            Piece* pieceAtNewPos = board->getPiece(newPos);
        
            if (pieceAtNewPos != nullptr && pieceAtNewPos->getColor() != color) {
                validMoves.push_back(newPos);
            } 
        }
    
    return validMoves;
}