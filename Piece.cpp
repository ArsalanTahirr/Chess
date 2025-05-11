#include <iostream>
#include <cctype>
#include <cstdlib>
#include "Piece.h"
#include "Position.h"
#include "Board.h"
#include "Enums.h"
#include "GlobalVariables.h"

Piece::Piece(Color color, const char character, const Position& position) :
color(color), position(position), isCaptured(false) {
    this->character = (color == Color::Black ? tolower(character): toupper(character));
    symbolForBoard = Piece::getPieceSymbol(this->character);
    switch(character) {
        case 'K':
        case 'k':
            type = PieceType::King; 
        break;
        case 'Q':
        case 'q': 
            type = PieceType::Queen; 
        break;
        case 'R':
        case 'r': 
            type = PieceType::Rook; 
        break;
        case 'B':
        case 'b': 
            type = PieceType::Bishop; 
        break;
        case 'N':
        case 'n': 
            type = PieceType::Knight; 
        break;
        case 'P':
        case 'p': 
            type = PieceType::Pawn; 
        break;
        default: 
            std::cerr << "Invalid piece character: " << character << std::endl;
            exit(1);
    }
    char file = 'a' + position.X;
    char rank = '1' + position.Y;
}

Piece::~Piece() {}

Color Piece::getColor() const { return color; }

PieceType Piece::getType() const { return type; }

bool Piece::getisCaptured() const { return isCaptured; }

void Piece::setisCaptured(bool captured) { isCaptured = captured; }

void Piece::setType(PieceType type) { this->type = type; }

Position Piece::getPosition() const { return position; }

void Piece::setPosition(const int X, const int Y) { 
    position.X = X; 
    position.Y = Y; 
    this->file = 'a' + X;
    this->rank = '1' + Y;
}

void Piece::setPosition(const Position& position) {
    this->position = position;
    this->file = 'a' + position.X;
    this->rank = '1' + position.Y;
}

std::string Piece::getSymbolForBoard() const { return symbolForBoard; }

bool Piece::canMove(const Position& to, Board* board) const {
    std::vector<Position> ValidMoves = getValidMoves(board);
    bool canMove = false;
    for(Position& position: ValidMoves) {
        if(position == to) {
            canMove = true;
            break;
        }
    }
    return canMove;
}

char Piece::getPieceCharacter() const { return character; }

std::string Piece::getPieceSymbol(char piece) {
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

bool Piece::isInBounds(const Position& pos) const {
    return (pos.X >= 0 && pos.X < BOARD_ROWS &&
            pos.Y >= 0 && pos.Y < BOARD_COLS);
}

bool Piece::isInBounds(int row, int column) const {
    return (row >= 0 && row < BOARD_ROWS && column >= 0 && column < BOARD_COLS);
}

bool Piece::isPiecePinnedDiagonally(Board* board) const {
    // Get the king's position for the piece's color
    Position kingPos = board->getKingPosition(color);

    // Check if the piece and king are on the same diagonal
    int deltaX = abs(kingPos.X - position.X);
    int deltaY = abs(kingPos.Y - position.Y);
    if (deltaX != deltaY) {
        return false; // Not on the same diagonal
    }

    // Determine the direction from piece to king
    int stepX = (kingPos.X > position.X) ? 1 : -1;
    int stepY = (kingPos.Y > position.Y) ? 1 : -1;

    // Check if there are any pieces between piece and king
    Position current = position;
    current.X += stepX;
    current.Y += stepY;
    while (current != kingPos) {
        if (board->getPiece(current) != nullptr) {
            return false;
        }
        current.X += stepX;
        current.Y += stepY;
    }

    // Check for opponent pinning pieces (bishop or queen) beyond the piece
    stepX *= -1;
    stepY *= -1;
    current = position;
    current.X += stepX;
    current.Y += stepY;
    while (isInBounds(current)) {
        Piece* piece = board->getPiece(current);
        if (piece != nullptr) {
            if (piece->getColor() != color &&
                (piece->getType() == PieceType::Bishop || piece->getType() == PieceType::Queen)) {
                return true; // Pinning piece found
            }
            break; // Non-pinning piece or same color piece blocks the pin
        }
        current.X += stepX;
        current.Y += stepY;
    }

    return false; // No pinning piece found
}

bool Piece::isPiecePinnedHorizontally(Board* board) const {
    // Get the king's position for the piece's color
    Position kingPos = board->getKingPosition(color);
    
    // Check if the piece and king are on the same row
    if (kingPos.Y != position.Y) {
        return false; // Not on the same row
    }

    // Check if there are any pieces between piece and king
    Position current = position;
    int stepX = (kingPos.X > position.X) ? 1 : -1;
    current.X += stepX;
    while (current != kingPos) {
        if (board->getPiece(current) != nullptr) {
            return false;
        }
        current.X += stepX;
    }

    // Check for opponent pinning pieces (rook or queen) beyond the piece
    stepX *= -1;
    current = position;
    current.X += stepX;
    while (isInBounds(current)) {
        Piece* piece = board->getPiece(current);
        if (piece != nullptr) {
            if (piece->getColor() != getColor() &&
                (piece->getType() == PieceType::Rook || piece->getType() == PieceType::Queen)) {
                return true; // Pinning piece found
            }
            break; // Non-pinning piece or same color piece blocks the pin
        }
        current.X += stepX;
    }

    return false; // No pinning piece found
}

bool Piece::isPiecePinnedVertically(Board* board) const {
    // Get the king's position for the piece's color
    Position kingPos = board->getKingPosition(color);
    
    // Check if the piece and king are on the same row
    if (kingPos.X != position.X) {
        return false; // Not on the same row
    }

    // Check if there are any pieces between piece and king
    Position current = position;
    int stepY = (kingPos.Y > position.Y) ? 1 : -1;
    current.Y += stepY;
    while (current != kingPos) {
        if (board->getPiece(current) != nullptr) {
            return false;
        }
        current.Y += stepY;
    }

    // Check for opponent pinning pieces (rook or queen) beyond the piece
    stepY *= -1;
    current = position;
    current.Y += stepY;
    while (isInBounds(current)) {
        Piece* piece = board->getPiece(current);
        if (piece != nullptr) {
            if (piece->getColor() != getColor() &&
                (piece->getType() == PieceType::Rook || piece->getType() == PieceType::Queen)) {
                return true; // Pinning piece found
            }
            break; // Non-pinning piece or same color piece blocks the pin
        }
        current.Y += stepY;
    }
    return false; // Pawns cannot be pinned vertically
}

char Piece::getFile() const { return file; }

char Piece::getRank() const { return rank; }

void Piece::setFile(char file) { this->file = file; }

void Piece::setRank(char rank) { this->rank = rank; }