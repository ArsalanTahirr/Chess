#include "Headers.h"

int main() {
    Board board;
    board.initializeBoard();
    board.printBoard();
    Player player1("Player 1", Color::White);
    Player player2("Player 2", Color::Black);
    Player *currentPlayer = &player1;
    int i = 0;
    
    while(true) {
        cout << currentPlayer->getName() << "'s turn" << endl;
        int fromRow, fromCol, toRow, toCol;
        cout << "Enter a piece (fromRow fromCol toRow toCol): ";
        cin >> fromRow >> fromCol;
        if(fromRow == -1 || fromCol == -1) {
            cout << "Exiting game." << endl;
            break;
        }
        Position from(fromRow, fromCol);
        
        Piece *piece = board.getPiece(from);
        vector<Position> possibleMoves = piece->getValidMoves(&board);
        cout << "Possible moves: ";
        for (const auto& move : possibleMoves) {
            cout << "(" << move.getRow() << ", " << move.getColumn() << ") ";
        }
        cout << endl;
        cout << "Enter destination (toRow toCol): ";
        cin >> toRow >> toCol;
        Position to(toRow, toCol);
        if (piece == nullptr) {
            cout << "No piece at selected position!" << endl;
            continue;
        }
        
        if (piece->getColor() != currentPlayer->getColor()) {
            cout << "That's not your piece!" << endl;
            continue;
        }
        
        if (piece->canMove(to, &board)) {
            // Remove piece from original position
            board.removePiece(from);
            // Place piece in new position
            board.placePiece(piece, to, *currentPlayer);
            board.printBoard();
            vector<Piece*> pieces = currentPlayer->getCapturedPieces();
            cout << "Captured pieces: ";
            for (const auto& capturedPiece : pieces) {
                cout << capturedPiece->getSymbolForBoard() << " ";
            }
            cout << endl;
            
            // Switch players
            currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;
            i++;
        } else {
            cout << "Invalid move. Try again." << endl;
        }
    }
    return 0;
}