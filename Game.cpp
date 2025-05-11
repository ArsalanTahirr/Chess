#include "Headers.h"
#include <iomanip>
#include <fstream>

// ASCII Art Chess logo with Unicode characters
void displayChessLogo() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    
    std::cout << std::endl;
    std::cout << "   ██████╗██╗  ██╗███████╗███████╗███████╗" << std::endl;
    std::cout << "  ██╔════╝██║  ██║██╔════╝██╔════╝██╔════╝" << std::endl;
    std::cout << "  ██║     ███████║█████╗  ███████╗███████╗" << std::endl;
    std::cout << "  ██║     ██╔══██║██╔══╝  ╚════██║╚════██║" << std::endl;
    std::cout << "  ╚██████╗██║  ██║███████╗███████║███████║" << std::endl;
    std::cout << "   ╚═════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝" << std::endl;
    std::cout << std::endl;
    
    SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

// Display a centered message
void displayCenteredMessage(const std::string& message, bool highlight = false) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    
    if (highlight) {
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    }
    
    std::cout << std::setw(40 + message.length()/2) << message << std::endl;
    
    if (highlight) {
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
}

Game::Game() : board(nullptr), whitePlayer(nullptr), blackPlayer(nullptr), 
    currentPlayer(nullptr), inputParser(nullptr), hasError(false), useMouseInput(true) {}

Game::~Game() {
    // Clean up dynamically allocated memory
    delete board;
    delete whitePlayer;
    delete blackPlayer;
    delete inputParser;
    moveHistory.clear(); // Clear the move history
}

void Game::setError(const std::string& error) {
    lastErrorMessage = error;
    hasError = true;
}

void Game::clearError() {
    lastErrorMessage.clear();
    hasError = false;
}

void Game::setMouseInputMode(bool mode) {
    useMouseInput = mode;
}

void Game::startGame() {
    // Clear the screen
    system("cls");
    
    // Display welcome screen
    displayChessLogo();
    displayCenteredMessage("Welcome to Chess", true);
    displayCenteredMessage("A classic two-player strategy game");
    std::cout << std::endl;
    
    // Initialize the game components
    board = new Board();
    board->initializeBoard();
    
    // Get player names
    std::string player1Name, player2Name;
    
    std::cout << "Enter name for White player: ";
    std::getline(std::cin, player1Name);
    if (player1Name.empty()) player1Name = "Player 1";
    
    std::cout << "Enter name for Black player: ";
    std::getline(std::cin, player2Name);
    if (player2Name.empty()) player2Name = "Player 2";
    
    whitePlayer = new Player(player1Name, Color::White);
    blackPlayer = new Player(player2Name, Color::Black);
    currentPlayer = whitePlayer; // White starts first
    inputParser = new InputParser(board);
    
    // Record initial board state
    moveHistory.push_back(StateString(*currentPlayer, board));
    
    // Display game instructions
    system("cls");
    displayChessLogo();
    displayCenteredMessage("Game Instructions", true);
    std::cout << std::endl;
    std::cout << " • White pieces (" << player1Name << ") move first" << std::endl;
    std::cout << " • Pieces move according to standard chess rules" << std::endl;
    std::cout << " • Type 'o-o' for kingside castling, 'o-o-o' for queenside castling" << std::endl;
    std::cout << " • For pawn promotion, use format 'e7e8=Q' (replace Q with desired piece)" << std::endl;
    
    if (useMouseInput) {
        std::cout << std::endl;
        std::cout << " Mouse Controls:" << std::endl;
        std::cout << " • Click on a piece to select it" << std::endl;
        std::cout << " • Click on a destination square to move" << std::endl;
        std::cout << " • For pawn promotion, you will be prompted to choose a piece" << std::endl;
    } else {
        std::cout << std::endl;
        std::cout << " Text Input Format:" << std::endl;
        std::cout << " • Enter moves in algebraic notation (e.g., 'e2e4')" << std::endl;
        std::cout << " • First two characters represent the starting position" << std::endl;
        std::cout << " • Last two characters represent the destination" << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "Press Enter to start the game...";
    // Clear input buffer and wait for Enter
    std::cin.clear();
    std::cin.ignore(9999, '\n');
}

void Game::playGame() {
    bool gameOver = false;
    
    while (!gameOver) {
        // Check game end conditions first
        // Check if current player is in checkmate
        bool isCurrentPlayerInCheckmate = ChessRules::isCheckmate(currentPlayer->getColor(), board);
        if(isCurrentPlayerInCheckmate) {
            board->printBoard();
            
            // Display checkmate message
            HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
            std::cout << std::endl;
            std::cout << " ♚ CHECKMATE! ♚" << std::endl;
            SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            
            std::cout << " " << currentPlayer->getName() << " is in checkmate!" << std::endl;
            
            // Display winner
            Player* winner = (currentPlayer == whitePlayer) ? blackPlayer : whitePlayer;
            SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << " " << winner->getName() << " wins the game!" << std::endl;
            SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            
            gameOver = true;
            break;
        }
        
        if(ChessRules::isStalemate(currentPlayer->getColor(), board)) {
            board->printBoard();
            
            // Display stalemate message
            HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << std::endl;
            std::cout << " ♖ STALEMATE! ♖" << std::endl;
            SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            
            std::cout << " " << currentPlayer->getName() << " has no legal moves!" << std::endl;
            std::cout << " The game is a draw!" << std::endl;
            
            gameOver = true;
            break;
        }

        std::pair<bool, std::string> drawResult = ChessRules::isDraw(moveHistory, board);

        if (drawResult.first) {
            board->printBoard();
            
            // Display draw message
            HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << std::endl;
            std::cout << " ♘ DRAW! ♘" << std::endl;
            SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            
            std::cout << " " << drawResult.second << std::endl;
            
            gameOver = true;
            break;
        }

        // Check for check
        bool isInCheck = ChessRules::isCheck(currentPlayer->getColor(), board);
        
        board->printBoard();
        
        // Display current player and check status
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        if (currentPlayer->getColor() == Color::White) {
            SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        } else {
            SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
        }
        
        std::cout << std::endl << " " << currentPlayer->getName() << "'s turn (";
        std::cout << (currentPlayer->getColor() == Color::White ? "White" : "Black") << ")" << std::endl;
        
        if (isInCheck) {
            SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
            std::cout << " YOUR KING IS IN CHECK! You must address this threat." << std::endl;
        }
        
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        
        if (hasError) {
            SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
            std::cout << " Error: " << lastErrorMessage << std::endl;
            SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            clearError();
        }

        Move move;
        
        // Prompt for move
        if (useMouseInput) {
            std::cout << " Please click on a piece to move..." << std::endl;
        } else {
            std::cout << " Enter your move: ";
        }
        
        // Use appropriate input method
        try {
            if (useMouseInput) {
                move = inputParser->takeMouseInput();
            } else {
                move = inputParser->takeInput();
            }
        }
        catch (std::invalid_argument &e) {
            std::string errorMsg = e.what();
            // Special case for the "Continue with selecting destination" message
            // which is used to indicate that we're waiting for the second click
            if (errorMsg == "Continue with selecting destination") {
                continue;
            }
            setError(errorMsg);
            continue; // Skip to the next iteration if input is invalid
        }

        bool moveSuccessful = false;
        if(move.getIsCastlingKingSide()) {
            try {
                ChessRules::handleCastlingKingSide(*currentPlayer, board);
                moveSuccessful = true;
            }
            catch (std::invalid_argument &e) {
                setError(e.what());
                continue; // Skip to the next iteration if input is invalid
            }
        } 
        else if(move.getIsCastlingQueenSide()) {
            try {
                ChessRules::handleCastlingQueenSide(*currentPlayer, board);
                moveSuccessful = true;
            }
            catch (std::invalid_argument &e) {
                setError(e.what());
                continue; // Skip to the next iteration if input is invalid
            }
        } 
        else if(move.getIsPromotion()) {
            PieceType type = move.getPieceType();
            try {
                ChessRules::handlePromotion(*currentPlayer, type, &move, board);
                moveSuccessful = true;
            }
            catch (std::invalid_argument &e) {
                setError(e.what());
                continue; // Skip to the next iteration if input is invalid
            }
        } 
        else {
            try {
                makeMove(move);
                moveSuccessful = true;
            }
            catch (std::invalid_argument &e) {
                setError(e.what());
                continue; // Skip to the next iteration if input is invalid
            }
        }
        
        // Only update the move history if the move was successful
        if (moveSuccessful) {
            // Add the current state to the move history
            moveHistory.push_back(StateString(*currentPlayer, board));
            
            // Explicitly check if the current player is now in checkmate after the move
            Player* opponent = (currentPlayer == whitePlayer) ? blackPlayer : whitePlayer;
            bool isOpponentInCheckmate = ChessRules::isCheckmate(opponent->getColor(), board);
            
            if (isOpponentInCheckmate) {
                board->printBoard();
                
                // Display checkmate message
                HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
                std::cout << std::endl;
                std::cout << " ♚ CHECKMATE! ♚" << std::endl;
                SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                
                std::cout << " " << opponent->getName() << " is in checkmate!" << std::endl;
                
                // Display winner
                SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                std::cout << " " << currentPlayer->getName() << " wins the game!" << std::endl;
                SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                
                gameOver = true;
                break;
            }
            
            // Switch players after successful move
            currentPlayer = (currentPlayer == whitePlayer) ? blackPlayer : whitePlayer;
        }
    }
    
    // End game message
    std::cout << std::endl;
    std::cout << "Game over! Press Enter to exit...";
    // Clear input buffer and wait for Enter
    std::cin.clear();
    std::cin.ignore(9999, '\n');
    std::cin.get(); // Added extra waiting for input to ensure the program doesn't exit prematurely
}

void Game::makeMove(Move& move) {
    Piece* pieceToMove = board->getPiece(move.getFrom());
    if (!pieceToMove) {
        throw std::invalid_argument("No piece at source position.");
    }
    
    if (pieceToMove->getColor() != currentPlayer->getColor()) {
        throw std::invalid_argument("Cannot move opponent's piece.");
    }

    if(pieceToMove->canMove(move.getTo(), board)) {
        // Save original position for logging
        Position fromPos = pieceToMove->getPosition();
        Position toPos = move.getTo();
        
        // Check for en passant capture
        if (pieceToMove->getType() == PieceType::Pawn) {
            Pawn* pawn = dynamic_cast<Pawn*>(pieceToMove);
            // If this is a diagonal move to an empty square, it might be an en passant capture
            if (fromPos.X != toPos.X && !board->isSquareOccupied(toPos)) {
                // Check if there's a pawn that can be captured via en passant
                Position capturedPawnPos(toPos.X, fromPos.Y);
                Piece* potentialCapture = board->getPiece(capturedPawnPos);
                if (potentialCapture && potentialCapture->getType() == PieceType::Pawn) {
                    Pawn* capturedPawn = dynamic_cast<Pawn*>(potentialCapture);
                    if (capturedPawn && capturedPawn->getCanCaptureWithEnPassant()) {
                        // Add the pawn to captured pieces
                        currentPlayer->addCapturedPiece(potentialCapture);
                        // Remove the captured pawn from the board
                        board->removePiece(capturedPawnPos);
                    }
                }
            }
        }
        
        // Execute the move
        board->movePiece(pieceToMove, toPos, *currentPlayer);
        
        // Reset all en passant flags on opponent pawns after move
        Color opponentColor = (currentPlayer->getColor() == Color::White) ? Color::Black : Color::White;
        std::vector<Piece*> opponentPieces = board->getAllPiecesOfColor(opponentColor);
        for (Piece* piece : opponentPieces) {
            if (piece->getType() == PieceType::Pawn) {
                Pawn* pawn = dynamic_cast<Pawn*>(piece);
                if (pawn) {
                    pawn->setCanCaptureWithEnPassant(false);
                }
            }
        }
    } 
    else {
        throw std::invalid_argument("Invalid move for the piece.");
    }
}