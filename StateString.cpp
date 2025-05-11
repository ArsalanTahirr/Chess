// #include <iostream>
// #include "StateString.h"
// #include "Player.h"
// #include "Board.h"
// #include "Piece.h"
// #include "Pawn.h"
// #include "ChessRules.h"
// #include "GlobalVariables.h"

// StateString::StateString(Player& player, Board* board) {
//     AddPiecePlacement(board);
//     std::cout << "Correct Placement" << std::endl;
//     stateString += " ";
//     AddCurrentPlayer(player);
//     std::cout << "Correct Player" << std::endl;
//     stateString += " ";
//     AddCastlingRights(board);
//     std::cout << "Correct Castling" << std::endl;
//     stateString += " ";
//     AddEnPassantRights(board);
//     std::cout << "Correct EnPassant" << std::endl;
//     stateString += " ";
// }

// StateString::~StateString() {}

// std::string StateString::getStateString() const {
//     return stateString;
// }

// void StateString::AddRowData(int row, Board* board) {
//     int empty = 0;
//     for (int col = 0; col < BOARD_COLS; col++) {
//         Piece* piece = board->getPiece(col, row);
        
//         if (piece == nullptr) {
//             empty++;
//         } else {
//             if (empty > 0) {
//                 stateString += std::to_string(empty);
//                 empty = 0;
//             }
//             stateString += piece->getPieceCharacter();
//         }
//     }
//     // Add any remaining empty squares at end of row
//     if (empty > 0) {
//         stateString += std::to_string(empty);
//     }
// }

// void StateString::AddPiecePlacement(Board* board) {
//     // Start from top row (7) to bottom row (0) for FEN notation
//     for (int row = 7; row >= 0; row--) {
//         AddRowData(row, board);
//         if (row > 0) {  // Don't add slash after last row
//             stateString += "/";
//         }
//     }
// }

// void StateString::AddCurrentPlayer(Player& player) {
//     stateString += (player.getColor() == Color::White) ? "w" : "b";
// }

// void StateString::AddCastlingRights(Board* board) {
//     bool castleWKS = ChessRules::canCastleKingSide(Color::White, board);
//     bool castleWQS = ChessRules::canCastleQueenSide(Color::White, board);
//     bool castleBKS = ChessRules::canCastleKingSide(Color::Black, board);
//     bool castleBQS = ChessRules::canCastleQueenSide(Color::Black, board);

//     if(!(castleWKS || castleWQS || castleBKS || castleBQS)) {
//         stateString += "-";
//         return;
//     } 
//     if (castleWKS) stateString += "K";
//     if (castleWQS) stateString += "Q";
//     if (castleBKS) stateString += "k";
//     if (castleBQS) stateString += "q";
// }

// void StateString::AddEnPassantRights(Board* board) {
//     std::vector<Piece*> pieces = board->getAllPieces();
//     Position enPassantPos;
//     for (Piece* piece : pieces) {
//         if (piece->getType() == PieceType::Pawn) {
//             Pawn* pawn = dynamic_cast<Pawn*>(piece);
//             if (pawn && pawn->getCanCaptureWithEnPassant()) {
//                 enPassantPos.setCoordinate(pawn->getPosition().X, 
//                 pawn->getPosition().Y + (pawn->getColor() == Color::White ? -1 : 1));
//                 break;
//             }
//         }
//     }

//     char file = 'a' + enPassantPos.X;
//     int rank = enPassantPos.Y + 1;

//     stateString += file;
//     stateString += std::to_string(rank);
// }