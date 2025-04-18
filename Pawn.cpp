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
    
    int direction = (color == Color::White) ? -1 : 1;
    
    Position oneStep(row + direction, col);
    if (isInBounds(oneStep) && board->getPiece(oneStep) == nullptr) {
        validMoves.push_back(oneStep);
        
        if (!hasMoved) {
            Position twoStep(row + (2 * direction), col);
            if (board->getPiece(twoStep) == nullptr) {
                validMoves.push_back(twoStep);
            }
        }
    }
    
    Position leftCapture(row + direction, col - 1);
    Position rightCapture(row + direction, col + 1);
    
    if (isInBounds(leftCapture)) {
        Piece* pieceAtLeft = board->getPiece(leftCapture);
        if (pieceAtLeft && pieceAtLeft->getColor() != color) {
            validMoves.push_back(leftCapture);
        }
    }
    
    if (isInBounds(rightCapture)) {
        Piece* pieceAtRight = board->getPiece(rightCapture);
        if (pieceAtRight && pieceAtRight->getColor() != color) {
            validMoves.push_back(rightCapture);
        }
    }
    
    return validMoves;
}