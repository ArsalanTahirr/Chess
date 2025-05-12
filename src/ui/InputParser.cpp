#include <iostream>
#include <string>
#include <exception>
#include <algorithm>
#include "../../include/ui/InputParser.h"
#include "../../include/core/Move.h"
#include "../../include/core/Board.h"
#include "../../include/core/Position.h"
#include "../../include/pieces/Piece.h"
#include "../../include/pieces/King.h"
#include "../../include/Enums.h"
#include "../../include/utils/GlobalVariables.h"



InputParser::InputParser(Board* board): board(board), firstClick(true) {}

InputParser::~InputParser() {}

Move InputParser::takeInput() {
    std::cout << "\n Enter Move: ";
    std::cin >> inputString;

    // Parse the input string into a Move object
    return parseInput();
}

Move InputParser::takeMouseInput() {
    Move move;
    
    if (firstClick) {
        // Get the first click position (piece to move)
        COORD clickPos = getConsoleClickPosition();
        
        // Clear any pending input
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        
        if (!handleMouseInput(clickPos, fromPosition)) {
            throw std::invalid_argument("Invalid source position selection");
        }
        
        // Check if a piece exists at from position
        Piece* pieceAtFrom = board->getPiece(fromPosition);
        if (!pieceAtFrom) {
            firstClick = true; // Reset to try again
            throw std::invalid_argument("No piece at selected position");
        }
        
        firstClick = false;
        
        // Report piece selection in a clean way
        std::cout << "\n Piece selected: " << char(fromPosition.X + 'a') << (fromPosition.Y + 1) << std::endl;
        
        // Add a small delay to prevent accidental double clicks
        Sleep(200);
        
        // At this point we wait for the second click which will happen in the next call to takeMouseInput
        throw std::invalid_argument("Continue with selecting destination");
    } else {
        // Get the second click position (destination)
        COORD clickPos = getConsoleClickPosition();
        
        // Clear any pending input
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        
        if (!handleMouseInput(clickPos, toPosition)) {
            firstClick = true; // Reset on error
            throw std::invalid_argument("Invalid destination position selection");
        }
        
        // Check that source and destination are different
        if (fromPosition == toPosition) {
            firstClick = true; // Reset on error
            throw std::invalid_argument("Source and destination positions cannot be the same");
        }
        
        // Check for special moves like castling or promotion
        Piece* pieceAtFrom = board->getPiece(fromPosition);
        
        // Display the move being made
        std::cout << "\n Move: " << char(fromPosition.X + 'a') << (fromPosition.Y + 1) 
                  << " to " << char(toPosition.X + 'a') << (toPosition.Y + 1) << std::endl;
        
        // Check for pawn promotion
        if (pieceAtFrom && pieceAtFrom->getType() == PieceType::Pawn) {
            if ((fromPosition.Y == 6 && toPosition.Y == 7) || 
                (fromPosition.Y == 1 && toPosition.Y == 0)) {
                
                char promotionPiece;
                std::cout << "\n Promote to (q=Queen, r=Rook, b=Bishop, n=Knight): ";
                std::cin >> promotionPiece;
                promotionPiece = std::tolower(promotionPiece);
                
                if (promotionPiece != 'q' && promotionPiece != 'r' &&
                    promotionPiece != 'b' && promotionPiece != 'n') {
                    firstClick = true; // Reset on error
                    throw std::invalid_argument("Invalid promotion piece type");
                }
                
                move.setIsPromotion(true);
                switch(promotionPiece) {
                    case 'q': move.setPieceType(PieceType::Queen); break;
                    case 'r': move.setPieceType(PieceType::Rook); break;
                    case 'b': move.setPieceType(PieceType::Bishop); break;
                    case 'n': move.setPieceType(PieceType::Knight); break;
                }
            }
        }
        
        // Set move properties
        move.setFrom(fromPosition);
        move.setTo(toPosition);
        move.setPieceMoved(pieceAtFrom);
        move.setPieceCaptured(board->getPiece(toPosition));
        
        // Check for castling moves
        if (pieceAtFrom && pieceAtFrom->getType() == PieceType::King) {
            // If king is moving two squares horizontally, it's a castling move
            if (abs(toPosition.X - fromPosition.X) == 2 && toPosition.Y == fromPosition.Y) {
                if (toPosition.X > fromPosition.X) {
                    // King is moving to the right - kingside castling
                    move.setIsCastlingKingSide(true);
                } else {
                    // King is moving to the left - queenside castling
                    move.setIsCastlingQueenSide(true);
                }
            }
        }
        
        firstClick = true; // Reset for next move
        return move;
    }
}

COORD InputParser::getConsoleClickPosition() {
    // Enable mouse input
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    
    INPUT_RECORD inputRecord;
    DWORD numEventsRead;
    COORD clickPos = {0, 0};
    
    while (true) {
        // Wait for an event
        ReadConsoleInput(hInput, &inputRecord, 1, &numEventsRead);
        
        // Only process mouse button click events (ignore mouse movement)
        if (inputRecord.EventType == MOUSE_EVENT) {
            // Only process LEFT BUTTON DOWN events (ignore movements and releases)
            if (inputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED &&
                inputRecord.Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
                
                clickPos = inputRecord.Event.MouseEvent.dwMousePosition;
                break;
            }
        }
    }
    
    // Restore previous console mode
    SetConsoleMode(hInput, prevMode);
    return clickPos;
}

Position InputParser::translateClickToChessPosition(COORD clickPos) {
    // Convert console coordinates to chess board coordinates
    // This depends on how the board is printed
    // We need to account for:
    // 1. The header row with a-h letters (2 rows)
    // 2. Each chess square has SQUARE_HEIGHT height
    // 3. The board is displayed with squares of width SQUARE_WIDTH
    // 4. There's a 3-character offset at the beginning of each row (rank number and spaces)
    
    // Board area top-left coordinate (estimate)
    const int boardStartY = 2;  // After header row with letters + blank line
    const int boardStartX = 3;  // After rank number and spacing
    
    // Calculate row and column on the chess board
    int boardRow = (clickPos.Y - boardStartY) / SQUARE_HEIGHT;
    int boardCol = (clickPos.X - boardStartX) / SQUARE_WIDTH;
    
    // The board is displayed with black at the top (7-0), we need to flip the Y coordinate
    int chessY = 7 - boardRow;
    int chessX = boardCol;
    
    // Validate board coordinates
    if (chessX < 0 || chessX > 7 || chessY < 0 || chessY > 7) {
        // Click is outside valid chess coordinates
        return Position(-1, -1);
    }
    
    return Position(chessX, chessY);
}

bool InputParser::handleMouseInput(COORD clickPos, Position& position) {
    position = translateClickToChessPosition(clickPos);
    
    // Check if the position is valid
    if (position.X < 0 || position.X > 7 || position.Y < 0 || position.Y > 7) {
        return false;
    }
    
    return true;
}

Move InputParser::parseInput() {
    Move move;
    transform(inputString.begin(), inputString.end(), inputString.begin(), ::tolower);

    // Check for castling
    if (inputString == "o-o" || inputString == "0-0") {
        move.setIsCastlingKingSide(true);
        return move;
    } 
    else if (inputString == "o-o-o" || inputString == "0-0-0") {
        move.setIsCastlingQueenSide(true);
        return move;
    }

    // Parse standard algebraic notation
    int inputLength = inputString.length();
    if(inputLength < 4) {
        throw std::invalid_argument("Invalid move format");
    }

    // Check for promotion
    if (inputString[inputLength - 2] == '=') {
        char promotionPiece = inputString[inputLength - 1];
        if (promotionPiece != 'q' && promotionPiece != 'r' &&
            promotionPiece != 'b' && promotionPiece != 'n') {
            throw std::invalid_argument("Invalid promotion piece type");
        }
        move.setIsPromotion(true);
        switch(promotionPiece) {
            case 'q': move.setPieceType(PieceType::Queen); break;
            case 'r': move.setPieceType(PieceType::Rook); break;
            case 'b': move.setPieceType(PieceType::Bishop); break;
            case 'n': move.setPieceType(PieceType::Knight); break;
        }
    }

    // Validate and create positions
    char fileFrom = inputString[0] - 'a';
    char rankFrom = inputString[1] - '1';
    char fileTo = inputString[2] - 'a';
    char rankTo = inputString[3] - '1';

    if (fileFrom < 0 || fileFrom > 7 || rankFrom < 0 || rankFrom > 7 ||
        fileTo < 0 || fileTo > 7 || rankTo < 0 || rankTo > 7) {
        throw std::invalid_argument("Invalid position coordinates");
    }

    Position fromPosition(fileFrom, rankFrom);
    Position toPosition(fileTo, rankTo);

    // Check if piece exists at from position
    Piece* pieceAtFrom = board->getPiece(fromPosition);
    if (!pieceAtFrom) {
        throw std::invalid_argument("No piece at source position");
    }

    // Check for pawn promotion requirement
    if((fromPosition.Y == 6 || fromPosition.Y == 1) && 
       (toPosition.Y == 7 || toPosition.Y == 0) &&
       pieceAtFrom->getType() == PieceType::Pawn && 
       inputLength != 7 && inputString[inputLength - 2] != '=') {
        throw std::invalid_argument("Pawn promotion move requires promotion piece specification");
    }

    move.setFrom(fromPosition);
    move.setTo(toPosition);
    move.setPieceMoved(pieceAtFrom);
    move.setPieceCaptured(board->getPiece(toPosition));

    return move;
}

