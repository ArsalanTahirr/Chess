#include <iostream>
#include <cmath>
#include "Pawn.h"
#include "GlobalVariables.h"

Pawn::Pawn(Color color, const char character, const Position& position):
Piece(color, character, position), hasMoved(false), canCaptureWithEnPassant(false) {}

Pawn::~Pawn() {}

void Pawn::setHasMoved(bool moved) {
    hasMoved = moved;
}

void Pawn::setCanCaptureWithEnPassant(bool canCapture) {
    canCaptureWithEnPassant = canCapture;
}

bool Pawn::getHasMoved() const {
    return hasMoved;
}

bool Pawn::getCanCaptureWithEnPassant() const {
    return canCaptureWithEnPassant;
}

std::vector<Position> Pawn::getValidMoves(Board* board) const {
    std::vector<Position> validMoves;
    if(isPiecePinnedDiagonally(board) || 
       isPiecePinnedHorizontally(board)) {
        return validMoves; // No valid moves if pinned
    }
    int row = position.X;
    int col = position.Y;
    
    int direction = (color == Color::White) ? 1 : -1;
    
    Position oneStep(row, col + direction);
    if (isInBounds(oneStep) && board->getPiece(oneStep) == nullptr) {
        validMoves.push_back(oneStep);
        
        if (!hasMoved) {
            Position twoStep(row, col + (2 * direction));
            if (board->getPiece(twoStep) == nullptr) {
                validMoves.push_back(twoStep);
            }
        }
    }
    
    Position leftCapture(row - 1, col + direction);
    Position rightCapture(row + 1, col + direction);
    
    if (isInBounds(leftCapture)) {
        Piece* pieceAtLeft = board->getPiece(leftCapture);
        Piece* enPassantPiece = board->getPiece(Position(row - 1, col));
        if (pieceAtLeft && pieceAtLeft->getColor() != color) {
            validMoves.push_back(leftCapture);
        } 
        else if (enPassantPiece && enPassantPiece->getColor() != color && 
                   dynamic_cast<Pawn*>(enPassantPiece)->getCanCaptureWithEnPassant()) {
            validMoves.push_back(Position(row - 1, col + direction));
        }
    }
    
    if (isInBounds(rightCapture)) {
        Piece* pieceAtRight = board->getPiece(rightCapture);
        Piece* enPassantPiece = board->getPiece(Position(row + 1, col));
        if (pieceAtRight && pieceAtRight->getColor() != color) {
            validMoves.push_back(rightCapture);
        } 
        else if (enPassantPiece && enPassantPiece->getColor() != color && 
                   dynamic_cast<Pawn*>(enPassantPiece)->getCanCaptureWithEnPassant()) {
            validMoves.push_back(Position(row + 1, col + direction));
        }
    }
    
    return validMoves;
}

bool Pawn::canAttack(const Position& to, Board* board) const {
    Piece* targetPiece = board->getPiece(to);
    if (targetPiece == nullptr) {
        return false; // No piece to attack
    }
    if (targetPiece->getColor() == color) {
        return false; // Cannot attack own piece
    }
    std::vector<Position> validMoves;

    int row = position.X;
    int col = position.Y;
    
    int direction = (color == Color::White) ? 1 : -1;
    
    Position leftCapture(row - 1, col + direction);
    Position rightCapture(row + 1, col + direction);
    
    if (isInBounds(leftCapture)) {
        Piece* pieceAtLeft = board->getPiece(leftCapture);
        Piece* enPassantPiece = board->getPiece(Position(row - 1, col));
        if (pieceAtLeft && pieceAtLeft->getColor() != color) {
            validMoves.push_back(leftCapture);
        } 
        else if (enPassantPiece && enPassantPiece->getColor() != color && 
                   dynamic_cast<Pawn*>(enPassantPiece)->getCanCaptureWithEnPassant()) {
            validMoves.push_back(Position(row - 1, col + direction));
        }
    }
    
    if (isInBounds(rightCapture)) {
        Piece* pieceAtRight = board->getPiece(rightCapture);
        Piece* enPassantPiece = board->getPiece(Position(row + 1, col));
        if (pieceAtRight && pieceAtRight->getColor() != color) {
            validMoves.push_back(rightCapture);
        } 
        else if (enPassantPiece && enPassantPiece->getColor() != color && 
                   dynamic_cast<Pawn*>(enPassantPiece)->getCanCaptureWithEnPassant()) {
            validMoves.push_back(Position(row + 1, col + direction));
        }
    }

    for (const Position& move : validMoves) {
        if (move == to) {
            return true; // Pawn can attack the target position
        }
    }

    return false; // Cannot attack the target position
}