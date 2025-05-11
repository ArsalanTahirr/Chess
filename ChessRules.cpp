#include <iostream>
#include "ChessRules.h"
#include "Board.h"
#include "Move.h"
#include "Piece.h"
#include "MoveHistory.h"
#include "Position.h"
#include "Enums.h"
#include "King.h"
#include "Rook.h"

ChessRules::ChessRules() {}

ChessRules::~ChessRules() {}

bool ChessRules::isCheck(Color& color, Board* board, MoveHistory* moveHistory) {
    if (!board) return false;
    
    Position kingPosition = board->getKingPosition(color);
    Piece* king = board->getPiece(kingPosition);
    
    if (!king) return false; // King not found, invalid state
    
    // Check all pieces of the opponent
    std::vector<Piece*> opponentPieces = board->getAllPiecesOfColor(color == Color::White ? Color::Black : Color::White);
    
    for (Piece* piece : opponentPieces) {
        if (piece->canAttack(kingPosition, board)) {
            return true; // King is in check
        }
    }
    
    return false; // King is not in check
    
}

// bool ChessRules::isCheckmate(Color& color, Board* board) {
//     // Step 1: Check if the king is in check
//     if (!isCheck(color, board)) {
//         return false; // Not in checkmate if not in check
//     }
    
//     // Step 2: Get all pieces of the player
//     std::vector<Piece*> playerPieces = board->getAllPiecesOfColor(color);
//     Position kingPosition = board->getKingPosition(color);

//     // Step 3: For each piece, check if any legal move can get out of check
//     for (Piece* piece : playerPieces) {
//         std::vector<Position> validMoves = piece->getValidMoves(board);
        
//         // Try each move to see if it resolves the check
//         for (const Position& targetPos : validMoves) {
//             // Make a temporary move
//             Position originalPos = piece->getPosition();
//             Piece* capturedPiece = board->getPiece(targetPos);
            
//             // Execute the move temporarily
//             board->movePieceTemp(piece, targetPos);
            
//             // Check if king is still in check after the move
//             bool stillInCheck = isCheck(color, board);
            
//             // Restore the board
//             board->movePieceTemp(piece, originalPos);
            
//             // If captured piece, restore it
//             if (capturedPiece) {
//                 board->movePieceTemp(capturedPiece, targetPos);
//             }
            
//             // If this move gets out of check, it's not checkmate
//             if (!stillInCheck) {
//                 return false;
//             }
//         }
//     }
    
//     // If we've tried all possible moves and none resolve the check, it's checkmate
//     return true;
// }

// bool ChessRules::isStalemate(Color& color, Board* board) {
//     // Check if the king is in check
//     if (isCheck(color, board)) {
//         return false; // Not a stalemate if in check
//     }
    
//     // Get all pieces of the player
//     std::vector<Piece*> playerPieces = board->getAllPiecesOfColor(color);
    
//     // Check if any piece has a legal move
//     for (Piece* piece : playerPieces) {
//         std::vector<Position> validMoves = piece->getValidMoves(board);
//         if (!validMoves.empty()) {
//             return false; // Found a legal move, not stalemate
//         }
//     }
    
//     // No legal moves found, and not in check: stalemate
//     return true;
// }

// bool ChessRules::isDraw(MoveHistory& moveHistory, Board* board) {
//     // Check 50-move rule
//     if (board->getHalfMoveClock() >= 100) { // 50 full moves = 100 half moves
//         return true;
//     }
    
//     // Check threefold repetition
//     if (hasThreefoldRepetition(board)) {
//         return true;
//     }
    
//     // Check insufficient material
//     if (hasInsufficientMaterial(board)) {
//         return true;
//     }
//     Color White = Color::White;
//     Color Black = Color::Black;
//     // Check stalemate (no legal moves but not in check)
//     if (isStalemate(White, board) || isStalemate(Black, board)) {
//         return true;
//     }
    
//     return false;
// }

// // Helper function to check for threefold repetition
// bool ChessRules::hasThreefoldRepetition(Board* board) {
//     // Get current position's hash
//     uint64_t currentHash = board->getPositionHash();
    
//     // Count occurrences in position history
//     int repetitionCount = 0;
    
//     for (const auto& hash : board->getPositionHistory()) {
//         if (hash == currentHash) {
//             repetitionCount++;
//         }
//     }
    
//     // Position appears for the third time
//     return repetitionCount >= 3;
// }

// Helper function to check for insufficient material
// bool ChessRules::hasInsufficientMaterial(Board* board) {
//     int whitePieces = 0;
//     int blackPieces = 0;
//     bool whiteHasBishop = false;
//     bool blackHasBishop = false;
//     bool whiteHasKnight = false;
//     bool blackHasKnight = false;
//     int whiteBishopSquareColor = -1; // -1 = none, 0 = white square, 1 = black square
//     int blackBishopSquareColor = -1;
    
//     // Count all pieces and their types
//     for (int row = 0; row < 8; row++) {
//         for (int col = 0; col < 8; col++) {
//             Piece* piece = board->getPiece(row, col);
//             if (piece == nullptr) {
//                 continue;
//             }
            
//             if (piece->getColor() == Color::White) {
//                 whitePieces++;
//                 if (piece->getType() == PieceType::Bishop) {
//                     whiteHasBishop = true;
//                     // Determine bishop's square color (row+col) % 2
//                     whiteBishopSquareColor = (row + col) % 2;
//                 } 
//                 else if (piece->getType() == PieceType::Knight) {
//                     whiteHasKnight = true;
//                 } 
//                 else if (piece->getType() != PieceType::King) {
//                     // If there's any other piece (queen, rook, pawn), material is sufficient
//                     return false;
//                 }
//             } 
//             else { // BLACK
//                 blackPieces++;
//                 if (piece->getType() == PieceType::Bishop) {
//                     blackHasBishop = true;
//                     // Determine bishop's square color
//                     blackBishopSquareColor = (row + col) % 2;
//                 } else if (piece->getType() == PieceType::Knight) {
//                     blackHasKnight = true;
//                 } else if (piece->getType() != PieceType::King) {
//                     // If there's any other piece (queen, rook, pawn), material is sufficient
//                     return false;
//                 }
//             }
//         }
//     }
    
//     // King vs King
//     if (whitePieces == 1 && blackPieces == 1) {
//         return true;
//     }
    
//     // King vs King and Bishop
//     if ((whitePieces == 1 && blackPieces == 2 && blackHasBishop) ||
//         (whitePieces == 2 && blackPieces == 1 && whiteHasBishop)) {
//         return true;
//     }
    
//     // King vs King and Knight
//     if ((whitePieces == 1 && blackPieces == 2 && blackHasKnight) ||
//         (whitePieces == 2 && blackPieces == 1 && whiteHasKnight)) {
//         return true;
//     }
    
//     // King and Bishop vs King and Bishop (bishops on same colored squares)
//     if (whitePieces == 2 && blackPieces == 2 && 
//         whiteHasBishop && blackHasBishop && 
//         whiteBishopSquareColor == blackBishopSquareColor) {
//         return true;
//     }
    
//     return false;
// }

// bool ChessRules::canCastleKingSide(Color color, Board* board) {
//     if (!board) return false;
    
//     const int rank = (color == Color::White) ? 0 : 7;
//     const int kingFile = 4;
//     const int rookFile = 7;
    
//     Piece* king = board->getPiece(kingFile, rank);
//     Piece* rook = board->getPiece(rookFile, rank);
    
//     if (!king || !rook) return false;
    
//     King* kingPiece = dynamic_cast<King*>(king);
//     Rook* rookPiece = dynamic_cast<Rook*>(rook);
    
//     return (kingPiece && rookPiece && 
//             kingPiece->canCastle() && 
//             !rookPiece->getHasMoved() &&
//             !isCheck(color, board) &&
//             ChessRules::isCastleLegal(color, board, 1));
// }

// bool ChessRules::canCastleQueenSide(Color color, Board* board) {
//     if (!board) return false;
    
//     const int rank = (color == Color::White) ? 0 : 7;
//     const int kingFile = 4;
//     const int rookFile = 0;
    
//     Piece* king = board->getPiece(kingFile, rank);
//     Piece* rook = board->getPiece(rookFile, rank);
    
//     if (!king || !rook) return false;
    
//     King* kingPiece = dynamic_cast<King*>(king);
//     Rook* rookPiece = dynamic_cast<Rook*>(rook);
    
//     return (kingPiece && rookPiece && 
//             kingPiece->canCastle() && 
//             !rookPiece->getHasMoved() &&
//             !isCheck(color, board) &&
//             ChessRules::isCastleLegal(color, board, -1));
// }

// bool ChessRules::isCastleLegal(Color color, Board* board, int moveDirection) {
//     if (!board) return false;
    
//     const int rank = (color == Color::White) ? 0 : 7;
//     const int kingFile = 4;
    
//     King* king = dynamic_cast<King*>(board->getPiece(kingFile, rank));
//     if (!king) return false;
    
//     // Check squares between king and rook
//     if (moveDirection == 1) { // Kingside
//         Position squaresBetween[] = { 
//             Position(5, rank), 
//             Position(6, rank) 
//         };
        
//         // Check if squares between are empty
//         for (const auto& pos : squaresBetween) {
//             if (board->isSquareOccupied(pos)) return false;
//         }
        
//         // Check if king can safely move through squares
//         Position originalPos = king->getPosition();
//         bool canCastle = true;
        
//         for (const auto& pos : squaresBetween) {
//             board->movePieceTemp(king, pos);
//             if (!king->canMove(pos, board)) {
//                 canCastle = false;
//                 break;
//             }
//         }
        
//         // Restore king position
//         board->movePieceTemp(king, originalPos);
//         return canCastle;
        
//     } else { // Queenside
//         Position squaresBetween[] = { 
//             Position(3, rank), 
//             Position(2, rank), 
//             Position(1, rank) 
//         };
        
//         // Check if squares between are empty
//         for (const auto& pos : squaresBetween) {
//             if (board->isSquareOccupied(pos)) return false;
//         }
        
//         // Check if king can safely move through squares
//         Position originalPos = king->getPosition();
//         bool canCastle = true;
        
//         for (const auto& pos : squaresBetween) {
//             board->movePieceTemp(king, pos);
//             if (!king->canMove(pos, board)) {
//                 canCastle = false;
//                 break;
//             }
//         }
        
//         // Restore king position
//         board->movePieceTemp(king, originalPos);
//         return canCastle;
//     }
// }

// // Helper function to check if there are any legal moves
// bool ChessRules::hasLegalMoves(Board* board) {
//     std::vector<Move> legalMoves = generateAllLegalMoves(board);
//     return !legalMoves.empty();
// }