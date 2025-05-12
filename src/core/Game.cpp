#include "../../include/Headers.h"
#include "../../include/ui/ChessUI.h"
#include <iomanip>
#include <fstream>

Game::Game() : board(nullptr), whitePlayer(nullptr), blackPlayer(nullptr), 
    currentPlayer(nullptr), inputParser(nullptr), ui(nullptr),
    hasError(false), useMouseInput(true) {}

Game::~Game() {
    // Clean up dynamically allocated memory
    delete board;
    delete whitePlayer;
    delete blackPlayer;
    delete inputParser;
    delete ui;
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
    // Initialize UI
    ui = new ChessUI();
    
    // Display welcome screen
    ui->displayWelcomeScreen();
    
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
    ui->displayGameInstructions(player1Name, player2Name, useMouseInput);
    
    // Clear input buffer and wait for Enter
    std::cin.clear();
    std::cin.ignore(9999, '\n');
}

void Game::playGame() {
    bool gameOver = false;
    bool waitingForSecondClick = false; // Add flag to track if we're waiting for second click
    
    while (!gameOver) {
        // Only check game end conditions and redraw if not waiting for second click
        if (!waitingForSecondClick) {
            // Check game end conditions
            if(ChessRules::isStalemate(currentPlayer->getColor(), board)) {
                // Clear the screen completely
                ui->clearScreen();
                
                // Print the board
                ui->displayBoard(board);
                
                // Display stalemate message
                ui->displayStalemate(currentPlayer);
                
                gameOver = true;
                break;
            }

            std::pair<bool, std::string> drawResult = ChessRules::isDraw(moveHistory, board);

            if (drawResult.first) {
                // Clear the screen completely
                ui->clearScreen();
                
                // Print the board
                ui->displayBoard(board);
                
                // Display draw message
                ui->displayDraw(drawResult.second);
                
                gameOver = true;
                break;
            }

            // Check for check
            bool isInCheck = ChessRules::isCheck(currentPlayer->getColor(), board);
            
            // Clear the screen completely
            ui->clearScreen();
            
            // Print the board
            ui->displayBoard(board);
            
            // Display current player and check status
            ui->displayCurrentPlayerInfo(currentPlayer, isInCheck);
            
            if (hasError) {
                ui->displayErrorMessage(lastErrorMessage);
                clearError();
            }
        }

        Move move;
        
        // Prompt for move if not waiting for second click
        if (!waitingForSecondClick) {
            ui->displayMovePrompt(useMouseInput);
        }
        
        // Use appropriate input method
        try {
            if (useMouseInput) {
                move = inputParser->takeMouseInput();
                // If we get here with waitingForSecondClick=true, it means we got the second click
                waitingForSecondClick = false;
            } else {
                move = inputParser->takeInput();
            }
        }
        catch (std::invalid_argument &e) {
            std::string errorMsg = e.what();
            // Special case for the "Continue with selecting destination" message
            if (errorMsg == "Continue with selecting destination") {
                waitingForSecondClick = true; // Set flag instead of using continue
                continue;
            }
            setError(errorMsg);
            waitingForSecondClick = false; // Reset flag on error
            continue; // Skip to the next iteration if input is invalid
        }

        bool moveSuccessful = false;
        if(move.getIsCastlingKingSide()) {
            try {
                ChessRules::handleCastlingKingSide(*currentPlayer, board);
                moveSuccessful = true;
                std::cout << "\n Kingside castling performed" << std::endl;
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
                std::cout << "\n Queenside castling performed" << std::endl;
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
            resetEnpassantAbility(board->getPiece(move.getTo()));
            moveHistory.push_back(StateString(*currentPlayer, board));
            
            // Explicitly check if the current player is now in checkmate after the move
            Player* opponent = (currentPlayer == whitePlayer) ? blackPlayer : whitePlayer;
            bool isOpponentInCheckmate = ChessRules::isCheckmate(opponent->getColor(), board);
            
            if (isOpponentInCheckmate) {
                // Clear the screen completely
                ui->clearScreen();
                
                // Print the board
                ui->displayBoard(board);
                
                // Display checkmate message
                ui->displayCheckmate(currentPlayer, opponent);
                
                gameOver = true;
                break;
            }
            
            // Switch players after successful move
            currentPlayer = (currentPlayer == whitePlayer) ? blackPlayer : whitePlayer;
        }
    }
    
    // End game message
    ui->displayGameOver("Press Enter to exit...");
    
    // Clear input buffer and wait for Enter
    std::cin.clear();
    std::cin.ignore(9999, '\n');
    std::cin.get(); // Added extra waiting for input to ensure the program doesn't exit prematurely
}

void Game::makeMove(Move& move) {
    Piece* pieceToMove = board->getPiece(move.getFrom());
    if(!pieceToMove) {
        throw std::invalid_argument("Cannot move opponent's piece.");
    }
    if (pieceToMove->getColor() != currentPlayer->getColor()) {
        throw std::invalid_argument("Cannot move opponent's piece.");
    }
    if (pieceToMove->canMove(move.getTo(),board)) {
        // Save original position for logging
        Position fromPos = pieceToMove->getPosition();
        Position toPos = move.getTo();

        Piece* capturedPiece = board->getPiece(toPos);

        board->movePieceTemp(pieceToMove, toPos);
        // Check if the move puts the player's own king in check
        bool putsOwnKingInCheck = ChessRules::isCheck(currentPlayer->getColor(), board);
        // Restore the board to its original state
        board->movePieceTemp(pieceToMove, fromPos);
        // Restore captured piece if any
        if (capturedPiece) {
            board->movePieceTemp(capturedPiece, toPos);
        }
        // If the move puts the player's own king in check, it's illegal
        if (putsOwnKingInCheck) {
            throw std::invalid_argument("This move would put your king in check.");
        }


        Piece* currentPiece = board->getPiece(move.getFrom());
        if (currentPiece && currentPiece->getType() == PieceType::Pawn && !capturedPiece && fromPos.X != toPos.X) {
            if(currentPlayer->getColor() == Color::White) {
                Position enPassantCapturePos(toPos.X, toPos.Y - 1);
                Piece* potentialCapture = board->getPiece(enPassantCapturePos);
                if (potentialCapture && potentialCapture->getType() == PieceType::Pawn) {
                    Pawn* capturedPawn = dynamic_cast<Pawn*>(potentialCapture);
                    if (capturedPawn && capturedPawn->getCanCaptureWithEnPassant()) {
                        board->removePiece(enPassantCapturePos);
                        currentPlayer->addCapturedPiece(potentialCapture);
                    }
                }
            } else {
                Position enPassantCapturePos(toPos.X, toPos.Y + 1);
                Piece* potentialCapture = board->getPiece(enPassantCapturePos);
                if (potentialCapture && potentialCapture->getType() == PieceType::Pawn) {
                    Pawn* capturedPawn = dynamic_cast<Pawn*>(potentialCapture);
                    if (capturedPawn && capturedPawn->getCanCaptureWithEnPassant()) {
                        board->removePiece(enPassantCapturePos);
                        currentPlayer->addCapturedPiece(potentialCapture);
                    }
                }
            }
        }
        board->movePiece(pieceToMove, move.getTo(), *currentPlayer);
    }
    else {
        throw std::invalid_argument("Invalid move for the piece.");
    }
}

void Game::resetEnpassantAbility(Piece* lastMovedPiece) {
    // Check if lastMovedPiece is valid
    if (!lastMovedPiece) {
        return;
    }
    
    // Reset the en passant ability for all pawns except the one that just moved
    std::vector<Piece*> allPieces = board->getAllPieces();
    for (Piece* piece : allPieces) {
        if (piece->getType() == PieceType::Pawn && piece != lastMovedPiece) {
            Pawn* pawn = dynamic_cast<Pawn*>(piece);
            if (pawn) {
                pawn->setCanCaptureWithEnPassant(false);
            }
        }
    }
}

