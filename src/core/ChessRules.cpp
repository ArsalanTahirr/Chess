#include <iostream>
#include <vector>
#include "../../include/core/ChessRules.h"
#include "../../include/core/Board.h"
#include "../../include/core/Move.h"
#include "../../include/pieces/Piece.h"
#include "../../include/core/Position.h"
#include "../../include/Enums.h"
#include "../../include/pieces/King.h"
#include "../../include/pieces/Rook.h"
#include "../../include/core/Player.h"
#include "../../include/core/StateString.h"
#include <fstream>

ChessRules::ChessRules() {}

ChessRules::~ChessRules() {}

bool ChessRules::isCheck(Color color, Board* board) {
    if (!board) return false;
    
    Position kingPosition = board->getKingPosition(color);
    Piece* king = board->getPiece(kingPosition);
    
    if (!king) return false; // King not found, invalid state
    
    // Check all pieces of the opponent
    std::vector<Piece*> opponentPieces = board->getAllPiecesOfColor(color == Color::White ? Color::Black : Color::White);
    
    for (Piece* piece : opponentPieces) {
        if (piece->canAttack(kingPosition, board)) {
            board->setHalfMoveClock(0); // Reset half move clock if king is in check
            return true; // King is in check
        }
    }
    
    return false; // King is not in check
    
}

bool ChessRules::isCheckmate(Color color, Board* board) {
    // Step 1: Check if the king is in check
    if (!isCheck(color, board)) {
        return false; // Not in checkmate if not in check
    }
    
    // Step 2: Get all pieces of the player
    std::vector<Piece*> playerPieces = board->getAllPiecesOfColor(color);
    Position kingPosition = board->getKingPosition(color);
    
    // Step 3: For each piece, check if any legal move can get out of check
    for (Piece* piece : playerPieces) {
        std::vector<Position> validMoves = piece->getValidMoves(board);
        
        // Try each move to see if it resolves the check
        for (const Position& targetPos : validMoves) {
            // Make a temporary move
            Position originalPos = piece->getPosition();
            Piece* capturedPiece = board->getPiece(targetPos);
            
            // Execute the move temporarily
            board->movePieceTemp(piece, targetPos);
            
            // Check if king is still in check after the move
            bool stillInCheck = isCheck(color, board);
            
            // Restore the board
            board->movePieceTemp(piece, originalPos);
            
            // If captured piece, restore it
            if (capturedPiece) {
                board->movePieceTemp(capturedPiece, targetPos);
            }
            
            // If this move gets out of check, it's not checkmate
            if (!stillInCheck) {
                return false;
            }
        }
    }
    
    // If we've tried all possible moves and none resolve the check, it's checkmate
    return true;
}

bool ChessRules::isStalemate(Color color, Board* board) {
    // Check if the king is in check
    if (isCheck(color, board)) {
        return false; // Not a stalemate if in check
    }
    
    // Get all pieces of the player
    std::vector<Piece*> playerPieces = board->getAllPiecesOfColor(color);
    
    // Check if any piece has a legal move
    for (Piece* piece : playerPieces) {
        std::vector<Position> validMoves = piece->getValidMoves(board);
        if (!validMoves.empty()) {
            return false; // Found a legal move, not stalemate
        }
    }
    
    // No legal moves found, and not in check: stalemate
    return true;
}

std::pair<bool, std::string> ChessRules::isDraw(std::vector<StateString>& stateHistory, Board* board) {
    // Check 50-move rule
    if (board->getHalfMoveClock() >= 100) { // 50 full moves = 100 half moves
        return {true, "Draw by 50-move rule"};
    }
    
    // Check threefold repetition
    if (hasThreefoldRepetition(stateHistory)) {
        return {true, "Draw by threefold repetition"};
    }

    // Check insufficient material
    if (hasInsufficientMaterial(board)) {
        return {true, "Draw by insufficient material"};
    }
    Color White = Color::White;
    Color Black = Color::Black;
    // Check stalemate (no legal moves but not in check)
    if (isStalemate(White, board) || isStalemate(Black, board)) {
        return {true, "Draw by stalemate"};
    }

    return {false, ""};
}

// Helper function to check for threefold repetition
bool ChessRules::hasThreefoldRepetition(std::vector<StateString>& stateHistory) {
    for(size_t i = 0; i < stateHistory.size(); i++) {
        int count = 1;  // Count includes the current position
        for(size_t j = 0; j < stateHistory.size(); j++) {
            if(i != j && stateHistory[i].getStateString() == stateHistory[j].getStateString()) {
                count++;
                if(count >= 3) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool ChessRules::hasInsufficientMaterial(Board* board) {
    int whitePieces = 0;
    int blackPieces = 0;
    bool whiteHasBishop = false;
    bool blackHasBishop = false;
    bool whiteHasKnight = false;
    bool blackHasKnight = false;
    int whiteBishopSquareColor = -1; // -1 = none, 0 = white square, 1 = black square
    int blackBishopSquareColor = -1;
    
    // Count all pieces and their types
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece* piece = board->getPiece(row, col);
            if (piece == nullptr) {
                continue;
            }
            
            if (piece->getColor() == Color::White) {
                whitePieces++;
                if (piece->getType() == PieceType::Bishop) {
                    whiteHasBishop = true;
                    // Determine bishop's square color (row+col) % 2
                    whiteBishopSquareColor = (row + col) % 2;
                } 
                else if (piece->getType() == PieceType::Knight) {
                    whiteHasKnight = true;
                } 
                else if (piece->getType() != PieceType::King) {
                    // If there's any other piece (queen, rook, pawn), material is sufficient
                    return false;
                }
            } 
            else { // BLACK
                blackPieces++;
                if (piece->getType() == PieceType::Bishop) {
                    blackHasBishop = true;
                    // Determine bishop's square color
                    blackBishopSquareColor = (row + col) % 2;
                } else if (piece->getType() == PieceType::Knight) {
                    blackHasKnight = true;
                } else if (piece->getType() != PieceType::King) {
                    // If there's any other piece (queen, rook, pawn), material is sufficient
                    return false;
                }
            }
        }
    }
    
    // King vs King
    if (whitePieces == 1 && blackPieces == 1) {
        return true;
    }
    
    // King vs King and Bishop
    if ((whitePieces == 1 && blackPieces == 2 && blackHasBishop) ||
        (whitePieces == 2 && blackPieces == 1 && whiteHasBishop)) {
        return true;
    }
    
    // King vs King and Knight
    if ((whitePieces == 1 && blackPieces == 2 && blackHasKnight) ||
        (whitePieces == 2 && blackPieces == 1 && whiteHasKnight)) {
        return true;
    }
    
    // King and Bishop vs King and Bishop (bishops on same colored squares)
    if (whitePieces == 2 && blackPieces == 2 && 
        whiteHasBishop && blackHasBishop && 
        whiteBishopSquareColor == blackBishopSquareColor) {
        return true;
    }
    
    return false;
}

bool ChessRules::canCastleKingSide(Color color, Board* board) {
    if (!board) return false;
    
    const int rank = (color == Color::White) ? 0 : 7;
    const int kingFile = 4;
    const int rookFile = 7;
    
    Piece* king = board->getPiece(Position(kingFile, rank));
    Piece* rook = board->getPiece(Position(rookFile, rank));
    
    if (!king || !rook) return false;
    
    King* kingPiece = dynamic_cast<King*>(king);
    Rook* rookPiece = dynamic_cast<Rook*>(rook);
    
    if (!kingPiece || !rookPiece || kingPiece->getColor() != color || rookPiece->getColor() != color) 
        return false;
    
    return (kingPiece->canCastle() && 
            !rookPiece->getHasMoved() &&
            !isCheck(color, board) &&
            ChessRules::isCastleLegal(color, board, 1));
}

bool ChessRules::canCastleQueenSide(Color color, Board* board) {
    if (!board) return false;
    
    const int rank = (color == Color::White) ? 0 : 7;
    const int kingFile = 4;  
    const int rookFile = 0;  
    
    Piece* king = board->getPiece(Position(kingFile, rank));
    Piece* rook = board->getPiece(Position(rookFile, rank));
    
    if (!king || !rook) return false;
    
    King* kingPiece = dynamic_cast<King*>(king);
    Rook* rookPiece = dynamic_cast<Rook*>(rook);
    
    if (!kingPiece || !rookPiece || kingPiece->getColor() != color || rookPiece->getColor() != color) 
        return false;
    
    return (kingPiece->canCastle() && 
            !rookPiece->getHasMoved() &&
            !isCheck(color, board) &&
            ChessRules::isCastleLegal(color, board, -1));
}

bool ChessRules::isCastleLegal(Color color, Board* board, int moveDirection) {
    if (!board) return false;
    
    const int rank = (color == Color::White) ? 0 : 7;
    const int kingFile = 4;
    
    Piece* kingPiece = board->getPiece(Position(kingFile, rank));
    if (!kingPiece) return false;
    
    King* king = dynamic_cast<King*>(kingPiece);
    if (!king || king->getColor() != color) return false;
    
    if (moveDirection == 1) { // Kingside
        // Check if squares between are empty
        for (int file = kingFile + 1; file < 7; ++file) {
            if (board->isSquareOccupied(Position(file, rank))) 
                return false;
        }
        
        // Check if king can safely move through squares (not in check)
        // Only need to check the two squares the king moves through
        for (int file = kingFile + 1; file <= kingFile + 2; ++file) {
            Position pos(file, rank);
            // Simulating the king at this position to check if it would be in check
            Position originalPos = king->getPosition();
            
            // Temporarily move the king
            board->movePieceTemp(king, pos);
            
            // Check if this position would put the king in check
            bool inCheck = isCheck(color, board);
            
            // Restore king position
            board->movePieceTemp(king, originalPos);
            
            if (inCheck) return false;
        }
        
        return true;
        
    } else { // Queenside
        // Check if squares between are empty
        for (int file = kingFile - 1; file > 0; --file) {
            if (board->isSquareOccupied(Position(file, rank))) 
                return false;
        }
        
        // Check if king can safely move through squares (not in check)
        // Only need to check the two squares the king moves through
        for (int file = kingFile - 1; file >= kingFile - 2; --file) {
            Position pos(file, rank);
            // Simulating the king at this position to check if it would be in check
            Position originalPos = king->getPosition();
            
            // Temporarily move the king
            board->movePieceTemp(king, pos);
            
            // Check if this position would put the king in check
            bool inCheck = isCheck(color, board);
            
            // Restore king position
            board->movePieceTemp(king, originalPos);
            
            if (inCheck) return false;
        }
        
        return true;
    }
}

void ChessRules::handleCastlingKingSide(Player& player, Board* board) {
    if(!canCastleKingSide(player.getColor(), board)) {
        throw std::invalid_argument("Castling is not allowed!");
    }
    if (player.getColor() == Color::White) {
        int halfMoveClock = board->getHalfMoveClock();
        board->movePiece(board->getPiece(Position(4, 0)), Position(6, 0), player);
        board->movePiece(board->getPiece(Position(7, 0)), Position(5, 0), player);
        board->setHalfMoveClock(halfMoveClock + 1);
    } else {
        int halfMoveClock = board->getHalfMoveClock();
        board->movePiece(board->getPiece(Position(4, 7)), Position(6, 7), player);
        board->movePiece(board->getPiece(Position(7, 7)), Position(5, 7), player);
        board->setHalfMoveClock(halfMoveClock + 1);
    }
}

void ChessRules::handleCastlingQueenSide(Player& player, Board* board) {
    if(!canCastleQueenSide(player.getColor(), board)) {
        throw std::invalid_argument("Castling is not allowed!");
    }
    if (player.getColor() == Color::White) {
        int halfMoveClock = board->getHalfMoveClock();
        board->movePiece(board->getPiece(Position(4, 0)), Position(2, 0), player);
        board->movePiece(board->getPiece(Position(0, 0)), Position(3, 0), player);
        board->setHalfMoveClock(halfMoveClock + 1);
    } else {
        int halfMoveClock = board->getHalfMoveClock();
        board->movePiece(board->getPiece(Position(4, 7)), Position(2, 7), player);
        board->movePiece(board->getPiece(Position(0, 7)), Position(3, 7), player);
        board->setHalfMoveClock(halfMoveClock + 1);
    }
}

void ChessRules::handlePromotion(Player& player, PieceType& type, Move* move, Board* board) {
    Color color = player.getColor();
    Piece *currentPiece = board->getPiece(move->getFrom());
    if(currentPiece->getType() != PieceType::Pawn) {
        throw std::invalid_argument("Promotion is only allowed for pawns!");
    }
    else if(currentPiece->getColor() != color) {
        throw std::invalid_argument("Promotion is only allowed for the player's own pieces!");
    }
    else if(!currentPiece->canMove(move->getTo(), board)) {
        throw std::invalid_argument("Promotion is only allowed for valid moves!");
    }
    board->removePiece(move->getFrom());
    delete currentPiece; // Remove the pawn being promoted
    Position pos = move->getTo();
    Piece *piece = board->getPiece(pos);
    if (piece) {
        piece->setisCaptured(true);
        player.addCapturedPiece(piece);
        board->removePiece(pos);
    }
    board->setPiece(type, color, pos);
    board->setHalfMoveClock(0); // Reset half move clock after promotion
}

