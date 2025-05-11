#include <iostream>
#include "Knight.h"
#include "GlobalVariables.h"

Knight::Knight(Color color, const char character, const Position& position):
Piece(color, character, position) {}

Knight::~Knight() {}

std::vector<Position> Knight::getValidMoves(Board* board) const {
    std::vector<Position> validMoves;
    int row = position.X;
    int col = position.Y;
    if(isPiecePinnedDiagonally(board) || isPiecePinnedHorizontally(board) || isPiecePinnedVertically(board)) {
        return validMoves;
    }
    
    int directions[8][2] = {
        {-2, -1}, {-2, 1},  // Up 2, left/right 1
        {2, -1},  {2, 1},   // Down 2, left/right 1
        {-1, -2}, {1, -2},  // Left 2, up/down 1
        {-1, 2},  {1, 2}    // Right 2, up/down 1
    };
    
    for (auto& dir : directions) {
        int newRow = row + dir[0];
        int newCol = col + dir[1];

        // Check board boundaries
        if (!isInBounds(newRow, newCol)) {
            continue;
        }
            
        Position newPos(newRow, newCol);
        Piece* pieceAtNewPos = board->getPiece(newPos);
        
        if (pieceAtNewPos == nullptr || pieceAtNewPos->getColor() != color) {
            validMoves.push_back(newPos);
        }
    }
    
    return validMoves;
}

bool Knight::canAttack(const Position& to, Board* board) const {
    std::vector<Position> validMoves;
    int row = position.X;
    int col = position.Y;
    
    int directions[8][2] = {
        {-2, -1}, {-2, 1},  // Up 2, left/right 1
        {2, -1},  {2, 1},   // Down 2, left/right 1
        {-1, -2}, {1, -2},  // Left 2, up/down 1
        {-1, 2},  {1, 2}    // Right 2, up/down 1
    };
    
    for (auto& dir : directions) {
        int newRow = row + dir[0];
        int newCol = col + dir[1];

        // Check board boundaries
        if (!isInBounds(newRow, newCol)) {
            continue;
        }
            
        Position newPos(newRow, newCol);
        Piece* pieceAtNewPos = board->getPiece(newPos);
        
        if (pieceAtNewPos == nullptr || pieceAtNewPos->getColor() != color) {
            validMoves.push_back(newPos);
        }
    }
    for (const auto& move : validMoves) {
        if (move == to) {
            return true;
        }
    }
    return false;
}