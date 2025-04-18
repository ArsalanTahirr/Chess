#include <iostream>
#include "Rook.h"
#include "GlobalVariables.h"

Rook::Rook(Color color, const char character, const Position& position):
Piece(color, character, position), hasMoved(false) {}

Rook::~Rook() {}

std::vector<Position> Rook::getValidMoves(Board* board) const {
    std::vector<Position> validMoves;
    int row = position.getRow();
    int col = position.getColumn();

    int directions[4][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };
    
    for (auto& dir : directions) {
        int newRow = row;
        int newCol = col;
        
        while (true) {
            newRow += dir[0];
            newCol += dir[1];
            
            if (!isInBounds(newRow, newCol)) {
                break;
            }
            
            Position newPos(newRow, newCol);
            Piece* pieceAtNewPos = board->getPiece(newPos);
            
            if (pieceAtNewPos == nullptr) {
                validMoves.push_back(newPos);
            } 
            else if (pieceAtNewPos->getColor() != color) {
                validMoves.push_back(newPos);
                break;
            } 
            else {
                break;
            }
        }
    }
    
    return validMoves;
}