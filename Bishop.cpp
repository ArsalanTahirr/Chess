#include <iostream>
#include "Bishop.h"
#include "GlobalVariables.h"

Bishop::Bishop(Color color, const char character, const Position& position):
Piece(color, character, position) {}

Bishop::~Bishop() {}

std::vector<Position> Bishop::getValidMoves(Board* board) const {
    std::vector<Position> validMoves;
    int row = position.getRow();
    int col = position.getColumn();

    int directions[4][2] = {
        {-1, -1}, {1, 1}, {1, -1}, {-1, 1}
    };
    
    for (auto& dir : directions) {
        int newRow = row;
        int newCol = col;
        
        while (true) {
            newRow += dir[0];
            newCol += dir[1];
            
            if (newRow < 0 || newRow >= BOARD_ROWS || 
                newCol < 0 || newCol >= BOARD_COLS) {
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