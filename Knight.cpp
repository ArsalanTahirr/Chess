#include <iostream>
#include "Knight.h"
#include "GlobalVariables.h"

Knight::Knight(Color color, const char character, const Position& position):
Piece(color, character, position) {}

Knight::~Knight() {}

std::vector<Position> Knight::getValidMoves(Board* board) const {
    std::vector<Position> validMoves;
    int row = position.getRow();
    int col = position.getColumn();

    int directions[4][2] = {
        {1, 2}, {-1, 2}, {-1, -2}, {1, -2}
    };
    
    for (auto& dir : directions) {
        int newRow = row;
        int newCol = col;
        newRow += dir[0];
        newCol += dir[1];

        if (newRow < 0 || newRow >= BOARD_ROWS || 
            newCol < 0 || newCol >= BOARD_COLS) {
            continue;
        }
            
        Position newPos(newRow, newCol);
        Piece* pieceAtNewPos = board->getPiece(newPos);
        
        if (pieceAtNewPos == nullptr) {
            validMoves.push_back(newPos);
        } 
        else if (pieceAtNewPos->getColor() != color) {
            validMoves.push_back(newPos);
        }
    }
    
    return validMoves;
}