#include <iostream>
#include <vector>
#include "Piece.h"
#include "Queen.h"
#include "Board.h"
#include "Position.h"
#include "GlobalVariables.h"

Queen::Queen(Color color, const char character, const Position& position):
Piece(color, character, position) {}

Queen::~Queen() {}

std::vector<Position> Queen::getValidMoves(Board* board) const {
    std::vector<Position> validMoves;
    int row = position.X;
    int col = position.Y;
    int **directions;
    int ROWS;
    int COLS;
    if(isPiecePinnedDiagonally(board)) {
        int directionX = (board->getKingPosition(getColor()).X > position.X) ? 1 : -1;
        int directionY = (board->getKingPosition(getColor()).Y > position.Y) ? 1 : -1;
        
        directions = new int*[2];
        for (int i = 0; i < 2; ++i) {
            directions[i] = new int[2];
            directions[i][0] = directionX * (i == 0 ? 1 : -1);
            directions[i][1] = directionY * (i == 0 ? 1 : -1);
        }
        ROWS = 2;
        COLS = 2;
    }
    else if(isPiecePinnedHorizontally(board)) {
        int directionX = (board->getKingPosition(getColor()).X > position.X) ? 1 : -1;

        for (int i = 0; i < 2; ++i) {
            directions[i] = new int[2];
            directions[i][0] = directionX * (i == 0 ? 1 : -1);
            directions[i][1] = 0;
        }
        ROWS = 2;
        COLS = 2;
    }
    else if(isPiecePinnedVertically(board)) {
        int directionY = (board->getKingPosition(getColor()).Y > position.Y) ? 1 : -1;

        for (int i = 0; i < 2; ++i) {
            directions[i] = new int[2];
            directions[i][0] = 0;
            directions[i][1] = directionY * (i == 0 ? 1 : -1);
        }
        ROWS = 2;
        COLS = 2;
    }
    else {
        directions = new int*[8];
        for (int i = 0; i < 8; ++i) {
            directions[i] = new int[2];
        }
        directions[0][0] = 0; directions[0][1] = 1; // Up
        directions[1][0] = 0; directions[1][1] = -1; // Down
        directions[2][0] = -1; directions[2][1] = 0; // Left
        directions[3][0] = 1; directions[3][1] = 0; // Right
        directions[4][0] = -1; directions[4][1] = 1; // Up-Left
        directions[5][0] = 1; directions[5][1] = 1; // Up-Right
        directions[6][0] = -1; directions[6][1] = -1; // Down-Left
        directions[7][0] = 1; directions[7][1] = -1; // Down-Right    
        ROWS = 8;
        COLS = 2;
    }

    for (int i = 0; i < ROWS; i++) {
        int* dir = directions[i];
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
    for(int i = 0; i < ROWS; i++) {
        delete[] directions[i];
    }
    delete[] directions;
    return validMoves;
}

bool Queen::canAttack(const Position& to, Board* board) const {
    std::vector<Position> validMoves;
    int row = position.X;
    int col = position.Y;
    int **directions;
    int ROWS;
    int COLS;

    directions = new int*[8];
    for (int i = 0; i < 8; ++i) {
        directions[i] = new int[2];
    }
    directions[0][0] = 0; directions[0][1] = 1; // Up
    directions[1][0] = 0; directions[1][1] = -1; // Down
    directions[2][0] = -1; directions[2][1] = 0; // Left
    directions[3][0] = 1; directions[3][1] = 0; // Right
    directions[4][0] = -1; directions[4][1] = 1; // Up-Left
    directions[5][0] = 1; directions[5][1] = 1; // Up-Right
    directions[6][0] = -1; directions[6][1] = -1; // Down-Left
    directions[7][0] = 1; directions[7][1] = -1; // Down-Right    
    ROWS = 8;
    COLS = 2;

    for (int i = 0; i < ROWS; i++) {
        int* dir = directions[i];
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
    for(int i = 0; i < ROWS; i++) {
        delete[] directions[i];
    }
    delete[] directions;
    for (const auto& move : validMoves) {
        if (move == to) {
            return true;
        }
    }
    return false;
}