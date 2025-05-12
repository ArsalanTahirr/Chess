#include <iostream>
#include "../../include/core/StateString.h"
#include "../../include/core/Player.h"
#include "../../include/core/Board.h"
#include "../../include/pieces/Piece.h"
#include "../../include/pieces/Pawn.h"
#include "../../include/core/ChessRules.h"
#include "../../include/utils/GlobalVariables.h"

StateString::StateString(Player& player, Board* board) {
    AddPiecePlacement(board);
    stateString += " ";
    AddCurrentPlayer(player);
    stateString += " ";
    AddCastlingRights(board);
    stateString += " ";
    AddEnPassantRights(board, player);
    stateString += " ";
}

StateString::~StateString() {}

std::string StateString::getStateString() const {
    return stateString;
}

void StateString::AddRowData(int row, Board* board) {
    int empty = 0;
    for (int col = 0; col < BOARD_COLS; col++) {
        Piece* piece = board->getPiece(col, row);
        
        if (piece == nullptr) {
            empty++;
        } else {
            if (empty > 0) {
                stateString += std::to_string(empty);
                empty = 0;
            }
            stateString += piece->getPieceCharacter();
        }
    }
    // Add any remaining empty squares at end of row
    if (empty > 0) {
        stateString += std::to_string(empty);
    }
}

void StateString::AddPiecePlacement(Board* board) {
    // Start from top row (7) to bottom row (0) for FEN notation
    for (int row = 7; row >= 0; row--) {
        AddRowData(row, board);
        if (row > 0) {  // Don't add slash after last row
            stateString += "/";
        }
    }
}

void StateString::AddCurrentPlayer(Player& player) {
    stateString += (player.getColor() == Color::White) ? "w" : "b";
}

void StateString::AddCastlingRights(Board* board) {
    bool castleWKS = ChessRules::canCastleKingSide(Color::White, board);
    bool castleWQS = ChessRules::canCastleQueenSide(Color::White, board);
    bool castleBKS = ChessRules::canCastleKingSide(Color::Black, board);
    bool castleBQS = ChessRules::canCastleQueenSide(Color::Black, board);
    if(!(castleWKS || castleWQS || castleBKS || castleBQS)) {
        stateString += "-";
        return;
    } 
    if (castleWKS) stateString += "K";
    if (castleWQS) stateString += "Q";
    if (castleBKS) stateString += "k";
    if (castleBQS) stateString += "q";
}

void StateString::AddEnPassantRights(Board* board, Player& player) {
    // Look for pawns that can be captured via en passant
    // These are opponent pawns that just moved two squares
    std::vector<Piece*> pieces = board->getAllPiecesOfColor(player.getColor() == Color::White ? Color::Black : Color::White);
    Position enPassantPos;
    bool flag = false;
    
    for (Piece* piece : pieces) {
        if (piece->getType() == PieceType::Pawn) {
            Pawn* pawn = dynamic_cast<Pawn*>(piece);
            if (pawn && pawn->getCanCaptureWithEnPassant()) {
                flag = true;
                Position pawnPos = piece->getPosition();
                
                // Calculate the en passant target square (behind the pawn)
                int direction = (piece->getColor() == Color::White) ? -1 : 1;
                enPassantPos.X = pawnPos.X;
                enPassantPos.Y = pawnPos.Y + direction;
                break;
            }
        }
    }
    
    if (!flag) {
        stateString += "-";
        return;
    }
    
    // Convert to algebraic notation (a-h for files, 1-8 for ranks)
    char file = 'a' + enPassantPos.X;
    int rank = enPassantPos.Y + 1;

    stateString += file;
    stateString += std::to_string(rank);
}

