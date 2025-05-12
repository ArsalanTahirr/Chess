#pragma once

#include <string>
#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif
#include "../core/Board.h"
#include "../core/Player.h"

// Forward declarations
class Board;
class Player;

/**
 * ChessUI class handles all user interface operations for the chess game
 * This provides an abstraction layer between the game logic and UI rendering
 */
class ChessUI {
private:
#ifdef _WIN32
    HANDLE console;
#endif
    bool useColor;

public:
    ChessUI();
    ~ChessUI();

    // General UI methods
    void clearScreen() const;
    void displayChessLogo() const;
    void displayCenteredMessage(const std::string& message, bool highlight = false) const;
    void displayWelcomeScreen() const;
    void displayGameInstructions(const std::string& whitePlayerName, const std::string& blackPlayerName, bool useMouseInput) const;
    void displayMainMenu() const;
    void displayIntro() const;
    
    // Game state display methods
    void displayBoard(const Board* board) const;
    void displayCurrentPlayerInfo(const Player* player, bool isInCheck) const;
    void displayErrorMessage(const std::string& errorMessage) const;
    void displayMovePrompt(bool useMouseInput) const;
    void displayGameOver(const std::string& message) const;
    void displayCheckmate(const Player* winner, const Player* loser) const;
    void displayStalemate(const Player* player) const;
    void displayDraw(const std::string& reason) const;
    
    // Console configuration methods
    void configureConsole() const;
    void setConsoleTitle() const;
    void setConsoleSize() const;
    
    // Helper methods
    void resetColor() const;
    void setColor(int foreground, int background) const;
    
    // Sleep function that works cross-platform
    void sleep(int milliseconds) const;
}; 