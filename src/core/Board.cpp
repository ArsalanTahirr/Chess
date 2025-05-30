#include <iostream>
#include <windows.h>
#include "../../include/core/Board.h"
#include "../../include/pieces/Piece.h"
#include "../../include/core/Position.h"
#include "../../include/core/Square.h"
#include "../../include/pieces/Rook.h"
#include "../../include/pieces/Knight.h"
#include "../../include/pieces/Bishop.h"
#include "../../include/pieces/Queen.h"
#include "../../include/pieces/King.h"
#include "../../include/pieces/Pawn.h"
#include "../../include/Enums.h"
#include "../../include/core/Player.h"
#include "../../include/utils/GlobalVariables.h"

Board::Board() : halfMoveClock(0) {
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            squares[i][j].position.setCoordinate(i, j);
            squares[i][j].piece = nullptr; // Initialize piece to nullptr
            if((i + j) % 2 == 0) squares[i][j].color = Color::Black;
            else squares[i][j].color = Color::White; 
        }
    }
}

Board::~Board() {
    for(int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            if(squares[i][j].piece != nullptr) {
                delete squares[i][j].piece; // Delete the piece object
            }
        }
    }
}

void Board::setHalfMoveClock(int clock) { halfMoveClock = clock; }

int Board::getHalfMoveClock() const { return halfMoveClock; }

void Board::setPiece(PieceType& pieceType, Color& color, const Position& position) {
    if(pieceType == PieceType::Bishop)
    squares[position.X][position.Y].piece = new Bishop(color, 'B', position);
    if(pieceType == PieceType::Pawn)
    squares[position.X][position.Y].piece = new Pawn(color, 'P', position);
    if(pieceType == PieceType::King)
    squares[position.X][position.Y].piece = new King(color, 'K', position);
    if(pieceType == PieceType::Knight)
    squares[position.X][position.Y].piece = new Knight(color, 'N', position);
    if(pieceType == PieceType::Queen)
    squares[position.X][position.Y].piece = new Queen(color, 'Q', position);
    if(pieceType == PieceType::Rook)
    squares[position.X][position.Y].piece = new Rook(color, 'R', position);
}

void Board::initializeBoard() {
    squares[0][7].piece = new Rook(Color::Black, 'r', Position(0, 7)); // Example piece placement
    squares[1][7].piece = new Knight(Color::Black, 'n', Position(1, 7)); // Example piece placement
    squares[2][7].piece = new Bishop(Color::Black, 'b', Position(2, 7)); // Example piece placement
    squares[3][7].piece = new Queen(Color::Black, 'q', Position(3, 7)); // Example piece placement
    squares[4][7].piece = new King(Color::Black, 'k', Position(4, 7)); // Example piece placement
    squares[5][7].piece = new Bishop(Color::Black, 'b', Position(5, 7)); // Example piece placement
    squares[6][7].piece = new Knight(Color::Black, 'n', Position(6, 7)); // Example piece placement
    squares[7][7].piece = new Rook(Color::Black, 'r', Position(7, 7)); // Example piece placement

    for(int i = 0; i < BOARD_COLS; i++) {
        squares[i][6].piece = new Pawn(Color::Black, 'p', Position(i, 6)); // Example piece placement
    }
    
    for(int i = 0; i < BOARD_COLS; i++) {
        squares[i][1].piece = new Pawn(Color::White, 'P', Position(i, 1)); // Example piece placement
    }
    
    squares[0][0].piece = new Rook(Color::White, 'R', Position(0, 0)); // Example piece placement
    squares[1][0].piece = new Knight(Color::White, 'N', Position(1, 0)); // Example piece placement
    squares[2][0].piece = new Bishop(Color::White, 'B', Position(2, 0)); // Example piece placement
    squares[3][0].piece = new Queen(Color::White, 'Q', Position(3, 0)); // Example piece placement
    squares[4][0].piece = new King(Color::White, 'K', Position(4, 0)); // Example piece placement
    squares[5][0].piece = new Bishop(Color::White, 'B', Position(5, 0)); // Example piece placement
    squares[6][0].piece = new Knight(Color::White, 'N', Position(6, 0)); // Example piece placement
    squares[7][0].piece = new Rook(Color::White, 'R', Position(7, 0)); // Example piece placement
}

Piece* Board::getPiece(const Position& position) const { return squares[position.X][position.Y].getPiece(); }

Piece* Board::getPiece(int row, int column) const { return squares[row][column].getPiece(); }

void Board::movePiece(Piece *piece, const Position& position, Player& player) {
    bool shouldIncrement = true;
    
    if(piece->getPieceCharacter() == 'P' || piece->getPieceCharacter() == 'p') {
        Pawn* pawn = dynamic_cast<Pawn*>(piece);
        if(pawn) {
            shouldIncrement = false; // Don't increment half move clock for pawn
            halfMoveClock = 0; // Reset half move clock for pawn
            pawn->setHasMoved(true); // Set hasMoved to true for pawn
            Position currentPos = pawn->getPosition();
            // Check if pawn moved two squares forward (different for white and black)
            if((pawn->getColor() == Color::White && position.Y - currentPos.Y == 2) || 
               (pawn->getColor() == Color::Black && currentPos.Y - position.Y == 2)) {
                pawn->setCanCaptureWithEnPassant(true); // Set canCaptureWithEnPassant to true for pawn
            }
        }
    } 
    else if(piece->getPieceCharacter() == 'R' || piece->getPieceCharacter() == 'r') {
        Rook* rook = dynamic_cast<Rook*>(piece);
        if(rook) {
            rook->setHasMoved(true); // Set hasMoved to true for rook
        }
    } 
    else if(piece->getPieceCharacter() == 'K' || piece->getPieceCharacter() == 'k') {
        King* king = dynamic_cast<King*>(piece);
        if(king) {
            king->setHasMoved(true); // Set hasMoved to true for king
        }
    }
    removePiece(piece->getPosition());
    if(isSquareOccupied(position)) {
        shouldIncrement = false;
        halfMoveClock = 0; // Reset half move clock if a piece is captured
        player.addCapturedPiece(getPiece(position)); // Add captured piece to player
        removePiece(position); // Remove existing piece if occupied
    }
    if(shouldIncrement) {
        halfMoveClock++; // Increment half move clock
    }
    squares[position.X][position.Y].setPiece(piece); 
    piece->setPosition(position.X, position.Y); // Update piece position
}

void Board::movePieceTemp(Piece *piece, const Position& position) {
    removePiece(piece->getPosition());
    if(isSquareOccupied(position)) {
        removePiece(position); // Remove existing piece if occupied
    }
    squares[position.X][position.Y].setPiece(piece); 
    piece->setPosition(position.X, position.Y); // Update piece position
}

void Board::removePiece(const Position& position) { squares[position.X][position.Y].setPiece(nullptr); }

bool Board::isSquareOccupied(const Position& position) const { return (getPiece(position) != nullptr);}

std::vector<Piece*> Board::getAllPieces() const {
    std::vector<Piece*> pieces;
    for(int i = 0; i < BOARD_ROWS; i++) {
        for(int j = 0; j < BOARD_COLS; j++) {
            Piece* piece = getPiece(i , j);
            if(piece != nullptr) {
                pieces.push_back(piece);
            }
        }
    }
    return pieces;
}

std::vector<Piece*> Board::getAllPiecesOfColor(Color color) const {
    std::vector<Piece*> pieces;
    for(int i = 0; i < BOARD_ROWS; i++) {
        for(int j = 0; j < BOARD_COLS; j++) {
            Piece* piece = getPiece(i , j);
            if(piece != nullptr && piece->getColor() == color) {
                pieces.push_back(piece);
            }
        }
    }
    return pieces;
}

void Board::setColor(HANDLE console, bool isWhiteSquare, bool isWhitePiece, bool hasPiece) const {
    int bg = isWhiteSquare ? WHITE_SQUARE_BG : BLACK_SQUARE_BG;
    int fg = isWhitePiece ? WHITE_PIECE_FG : BLACK_PIECE_FG;
    
    if (hasPiece) {
        SetConsoleTextAttribute(console, fg | bg);
    } else {
        SetConsoleTextAttribute(console, bg);
    }
}

void Board::resetColor(HANDLE console) const {
    SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Board::printBoard() const {
    // Don't clear the screen here, it's handled by the Game class
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set console code page to support Unicode characters
    SetConsoleOutputCP(CP_UTF8);
    
    // Print file letters (column headers)
    std::cout << "   ";
    for (int col = 0; col < 8; col++) {
        char file = 'a' + col;
        // Center the file letter in the square width
        for (int i = 0; i < SQUARE_WIDTH / 2 - 1; i++) {
            std::cout << " ";
        }
        std::cout << file;
        for (int i = 0; i < SQUARE_WIDTH / 2; i++) {
            std::cout << " ";
        }
    }
    std::cout << std::endl << std::endl;
    
    // Print board with pieces
    for (int row = 0; row < 8; row++) {
        // For each row of the board, print SQUARE_HEIGHT lines
        for (int h = 0; h < SQUARE_HEIGHT; h++) {
            // Print rank number at start of middle row
            if (h == SQUARE_HEIGHT / 2) {
                std::cout << 8 - row << " ";
            } else {
                std::cout << "  ";
            }
            
            // Print a line of the board
            for (int col = 0; col < 8; col++) {
                // Determine square and piece colors
                bool isWhiteSquare = (squares[col][7 - row].getColor() == Color::White);
                Piece *piece = getPiece(Position(col, 7 - row));
                bool hasPiece = (piece != nullptr);
                bool isWhitePiece = false;
                if(hasPiece) {
                    isWhitePiece = (piece->getColor() == Color::White);
                }
                
                // Set appropriate colors
                setColor(console, isWhiteSquare, isWhitePiece, hasPiece);
                
                // Print the square content
                if (h == SQUARE_HEIGHT / 2) {
                    // Middle row - print piece centered in square
                    std::string pieceSymbol;
                    if(piece) {
                        pieceSymbol = piece->getSymbolForBoard();
                    }
                    else {
                        pieceSymbol = " "; // Empty square
                    }
                    int paddingLeft = (SQUARE_WIDTH - 1) / 2;
                    int paddingRight = SQUARE_WIDTH - 1 - paddingLeft;
                    
                    for (int i = 0; i < paddingLeft; i++) {
                        std::cout << " ";
                    }
                    std::cout << pieceSymbol;
                    for (int i = 0; i < paddingRight; i++) {
                        std::cout << " ";
                    }
                } else {
                    // Top or bottom row - just print empty colored square
                    for (int i = 0; i < SQUARE_WIDTH; i++) {
                        std::cout << " ";
                    }
                }
                
                // Reset color at the end of each square
                resetColor(console);
            }
            
            // Print rank number at end of middle row
            if (h == SQUARE_HEIGHT / 2) {
                std::cout << " " << 8 - row;
            }
            std::cout << std::endl;
        }
    }
    
    // Print file letters again (column footers)
    std::cout << std::endl << "   ";
    for (int col = 0; col < 8; col++) {
        char file = 'a' + col;
        // Center the file letter in the square width
        for (int i = 0; i < SQUARE_WIDTH / 2 - 1; i++) {
            std::cout << " ";
        }
        std::cout << file;
        for (int i = 0; i < SQUARE_WIDTH / 2; i++) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
    
    // Add extra spacing after the board to prevent text overlap
    std::cout << std::endl << std::endl;
}

Position Board::getKingPosition(Color color) const {
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            Piece* piece = getPiece(i, j);
            if (piece && piece->getPieceCharacter() == (color == Color::White ? 'K' : 'k')) {
                return piece->getPosition();
            }
        }
    }
    return Position(-1, -1); // Return an invalid position if not found, not possible in a valid game
}

std::vector<Piece*> Board::getAllPiecesOfColorAndType(Color color, PieceType type) const {
    std::vector<Piece*> pieces;
    for(int i = 0; i < BOARD_ROWS; i++) {
        for(int j = 0; j < BOARD_COLS; j++) {
            Piece* piece = getPiece(i , j);
            if(piece != nullptr && piece->getColor() == color && piece->getType() == type) {
                pieces.push_back(piece);
            }
        }
    }
    return pieces;
}

