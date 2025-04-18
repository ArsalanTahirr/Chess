#include <iostream>
#include <windows.h>
#include "Board.h"
#include "Piece.h"
#include "Position.h"
#include "Square.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Pawn.h"
#include "Enums.h"
#include "Player.h"
#include "GlobalVariables.h"

Board::Board() {}

Board::~Board() {
    for(int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            delete squares[i][j].piece;
        }
    }
}

void Board::initializeBoard() {
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            squares[i][j].position.setCoordinate(i, j);
            if((i + j) % 2 == 0) squares[i][j].color = Color::Black;
            else squares[i][j].color = Color::White; 
        }
    }
    squares[0][0].piece = new Rook(Color::Black, 'r', Position(0, 0)); // Example piece placement
    squares[0][1].piece = new Knight(Color::Black, 'n', Position(0, 1)); // Example piece placement
    squares[0][2].piece = new Bishop(Color::Black, 'b', Position(0, 2)); // Example piece placement
    squares[0][3].piece = new Queen(Color::Black, 'q', Position(0, 3)); // Example piece placement
    squares[0][4].piece = new King(Color::Black, 'k', Position(0, 4)); // Example piece placement
    squares[0][5].piece = new Bishop(Color::Black, 'b', Position(0, 5)); // Example piece placement
    squares[0][6].piece = new Knight(Color::Black, 'n', Position(0, 6)); // Example piece placement
    squares[0][7].piece = new Rook(Color::Black, 'r', Position(0, 7)); // Example piece placement
    for(int i = 0; i < BOARD_COLS; i++) {
        squares[1][i].piece = new Pawn(Color::Black, 'p', Position(1, i)); // Example piece placement
    }
    for(int i = 0; i < BOARD_COLS; i++) {
        squares[6][i].piece = new Pawn(Color::White, 'P', Position(6, i)); // Example piece placement
    }
    squares[7][0].piece = new Rook(Color::White, 'R', Position(7, 0)); // Example piece placement
    squares[7][1].piece = new Knight(Color::White, 'N', Position(7, 1)); // Example piece placement
    squares[7][2].piece = new Bishop(Color::White, 'B', Position(7, 2)); // Example piece placement
    squares[7][3].piece = new Queen(Color::White, 'Q', Position(7, 3)); // Example piece placement
    squares[7][4].piece = new King(Color::White, 'K', Position(7, 4)); // Example piece placement
    squares[7][5].piece = new Bishop(Color::White, 'B', Position(7, 5)); // Example piece placement
    squares[7][6].piece = new Knight(Color::White, 'N', Position(7, 6)); // Example piece placement
    squares[7][7].piece = new Rook(Color::White, 'R', Position(7, 7)); // Example piece placement
}

Piece* Board::getPiece(const Position& position) const { return squares[position.getRow()][position.getColumn()].getPiece(); }

void Board::placePiece(Piece *piece, const Position& position, Player& player) {
    if(isSquareOccupied(position)) {
        player.addCapturedPiece(getPiece(position)); // Add captured piece to player
        removePiece(position); // Remove existing piece if occupied
    }
    squares[position.getRow()][position.getColumn()].setPiece(piece); 
    piece->setPosition(position.getRow(), position.getColumn()); // Update piece position
}

void Board::removePiece(const Position& position) { squares[position.getRow()][position.getColumn()].setPiece(nullptr); }

bool Board::isSquareOccupied(const Position& position) const { return getPiece(position);}

std::vector<Piece*> Board::getAllPieces() const {
    std::vector<Piece*> pieces;
    for(int i = 0; i < BOARD_ROWS; i++) {
        for(int j = 0; j < BOARD_COLS; j++) {
            Piece* piece = getPiece(Position(i , j));
            if(piece != nullptr) {
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
    system("cls"); // Clear the console
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
                bool isWhiteSquare = (squares[row][col].getColor() == Color::White);
                Piece *piece = getPiece(Position(row, col));
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
}