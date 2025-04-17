#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

// Define colors
const int BLACK_SQUARE_BG = BACKGROUND_RED | BACKGROUND_GREEN;          // Brown (red + green)
const int WHITE_SQUARE_BG = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY; // Light brown
const int BLACK_PIECE_FG = 0;                                           // Black
const int WHITE_PIECE_FG = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; // Bright white

// Define square size
const int SQUARE_WIDTH = 8;  // Width of each square in characters
const int SQUARE_HEIGHT = 3; // Height of each square in rows

// Initial board setup with pieces
// Capital letters are white pieces, lowercase are black pieces
// K/k=King, Q/q=Queen, R/r=Rook, B/b=Bishop, N/n=Knight, P/p=Pawn, ' '=Empty square
std::vector<std::vector<char>> initializeBoard() {
    std::vector<std::vector<char>> board = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };
    return board;
}

// Maps pieces to Unicode chess symbols
std::string getPieceSymbol(char piece) {
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

// Set console color based on square color and piece color
void setColor(HANDLE console, bool isWhiteSquare, bool isWhitePiece, bool hasPiece) {
    int bg = isWhiteSquare ? WHITE_SQUARE_BG : BLACK_SQUARE_BG;
    int fg = isWhitePiece ? WHITE_PIECE_FG : BLACK_PIECE_FG;
    
    if (hasPiece) {
        SetConsoleTextAttribute(console, fg | bg);
    } else {
        SetConsoleTextAttribute(console, bg);
    }
}

// Reset console color to default
void resetColor(HANDLE console) {
    SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

// Main function to print the chess board
void printBoard(const std::vector<std::vector<char>>& board) {
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
                bool isWhiteSquare = (row + col) % 2 != 1;
                char piece = board[row][col];
                bool hasPiece = (piece != ' ');
                bool isWhitePiece = (piece >= 'A' && piece <= 'Z');
                
                // Set appropriate colors
                setColor(console, isWhiteSquare, isWhitePiece, hasPiece);
                
                // Print the square content
                if (h == SQUARE_HEIGHT / 2) {
                    // Middle row - print piece centered in square
                    std::string pieceSymbol = getPieceSymbol(piece);
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

int main() {
    // Make sure console can display Unicode characters
    system("chcp 65001");
    
    // Clear the console
    system("cls");
    
    // Initialize and print the chess board
    std::vector<std::vector<char>> board = initializeBoard();
    printBoard(board);
    
    // Keep console window open until user presses Enter
    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    
    return 0;
}