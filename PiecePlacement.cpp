// #include "Headers.h"
// using namespace std;

// bool isCheck(Color& color, Board* board) {
//     // Check if the king of the given color is in check
//     cout << "Bookmark 2" << endl;
//     Position kingPos = board->getKingPosition(color);
//     std::vector<Piece*> AllPieces = board->getAllPiecesOfColor((color == Color::White) ? Color::Black : Color::White);
//     std::vector<Position> attackPositions;
//     cout << "Bookmark 3" << endl;
//     for(Piece* piece : AllPieces) {
//         if(piece->canMove(kingPos, board)) {
//             return true;
//         }
//     }
//     cout << "Bookmark 4" << endl;
//     return false;
// }

// bool canCastleKingSide(Color color, Board* board) {
//     if (!board) return false;
    
//     const int rank = (color == Color::White) ? 0 : 7;
//     const int kingFile = 4;
//     const int rookFile = 7;
    
//     Piece* king = board->getPiece(kingFile, rank);
//     Piece* rook = board->getPiece(rookFile, rank);
    
//     if (!king || !rook) return false;
    
//     King* kingPiece = dynamic_cast<King*>(king);
//     Rook* rookPiece = dynamic_cast<Rook*>(rook);
//     cout << "Bookmark 1" << endl;
//     return (kingPiece && rookPiece && 
//             kingPiece->canCastle() && 
//             !rookPiece->getHasMoved() &&
//             !isCheck(color, board) &&
//             ChessRules::isCastleLegal(color, board, 1));
// }

// bool canCastleQueenSide(Color color, Board* board) {
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

// bool isCastleLegal(Color color, Board* board, int moveDirection) {
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

// void AddCastlingRights(std::string& stateString, Board* board) {
//     bool castleWKS = canCastleKingSide(Color::White, board);
//     cout << "Castle White King Side: " << castleWKS << endl;
//     bool castleWQS = canCastleQueenSide(Color::White, board);
//     cout << "Castle White Queen Side: " << castleWQS << endl;
//     bool castleBKS = canCastleKingSide(Color::Black, board);
//     cout << "Castle Black King Side: " << castleBKS << endl;
//     bool castleBQS = canCastleQueenSide(Color::Black, board);
//     cout << "Castle Black Queen Side: " << castleBQS << endl;

//     if(!(castleWKS || castleWQS || castleBKS || castleBQS)) {
//         stateString += "-";
//         return;
//     } 
//     if (castleWKS) stateString += "K";
//     if (castleWQS) stateString += "Q";
//     if (castleBKS) stateString += "k";
//     if (castleBQS) stateString += "q";
// }

// int main() {
//     Board board;
//     board.initializeBoard();
//     std::cout << "Board initialized." << std::endl;
//     Player player1("Player 1", Color::White);
//     Player player2("Player 2", Color::Black);
//     std::cout << "Players initialized." << std::endl;
//     std::cout << "Creating StateString..." << std::endl;
//     std::string stateString;
//     AddCastlingRights(stateString , &board);
//     std::cout << "Initial State String: " << stateString << std::endl;
//     return 0;
// }