#pragma once

#include <string>
// Platform-specific includes for console handling
#ifdef _WIN32
#include <windows.h>  // Windows Console API
#else
#include <termios.h>  // Terminal I/O for Unix-like systems
#include <unistd.h>   // POSIX API
#endif
#include "../core/Board.h"
#include "../core/Player.h"

// Forward declarations to avoid circular dependencies
class Board;
class Player;

/**
 * ChessUI class handles all user interface operations for the chess game
 * 
 * This class provides a complete abstraction layer between the game logic and UI rendering,
 * implementing platform-specific code for both Windows and Unix-like systems.
 * It handles console configuration, text formatting, color output, and all user-facing
 * display elements of the chess game.
 */
class ChessUI {
private:
#ifdef _WIN32
    HANDLE console;  // Windows console handle for color and formatting
#endif
    bool useColor;   // Flag indicating whether color output is supported/enabled

public:
    /**
     * Constructor - initializes the UI system and detects platform capabilities
     */
    ChessUI();
    
    /**
     * Destructor - cleans up any platform-specific resources
     */
    ~ChessUI();

    // General UI methods - handle basic screen elements and navigation
    /**
     * Clears the console screen in a platform-independent way
     */
    void clearScreen() const;
    
    /**
     * Displays the chess game logo with appropriate styling
     */
    void displayChessLogo() const;
    
    /**
     * Shows a message centered on screen with optional highlighting
     */
    void displayCenteredMessage(const std::string& message, bool highlight = false) const;
    
    /**
     * Displays the welcome screen with game title and initial instructions
     */
    void displayWelcomeScreen() const;
    
    /**
     * Shows detailed game instructions based on the selected input mode
     */
    void displayGameInstructions(const std::string& whitePlayerName, const std::string& blackPlayerName, bool useMouseInput) const;
    
    /**
     * Displays the main menu with input method options
     */
    void displayMainMenu() const;
    
    /**
     * Shows the intro animation/splash screen
     */
    void displayIntro() const;
    
    // Game state display methods - show the current state of the game
    /**
     * Renders the chess board with pieces in their current positions
     */
    void displayBoard(const Board* board) const;
    
    /**
     * Shows information about the current player's turn and check status
     */
    void displayCurrentPlayerInfo(const Player* player, bool isInCheck) const;
    
    /**
     * Displays error messages with appropriate formatting
     */
    void displayErrorMessage(const std::string& errorMessage) const;
    
    /**
     * Shows a prompt for the player to enter their move
     */
    void displayMovePrompt(bool useMouseInput) const;
    
    /**
     * Displays game over message with the final result
     */
    void displayGameOver(const std::string& message) const;
    
    /**
     * Shows checkmate notification with winner information
     */
    void displayCheckmate(const Player* winner, const Player* loser) const;
    
    /**
     * Displays stalemate notification
     */
    void displayStalemate(const Player* player) const;
    
    /**
     * Shows draw notification with the reason for the draw
     */
    void displayDraw(const std::string& reason) const;
    
    // Console configuration methods - platform-specific terminal setup
    /**
     * Configures the console for optimal display (Unicode, colors, etc.)
     */
    void configureConsole() const;
    
    /**
     * Sets the console window title
     */
    void setConsoleTitle() const;
    
    /**
     * Adjusts the console window size for optimal game display
     */
    void setConsoleSize() const;
    
    // Helper methods - utility functions for UI rendering
    /**
     * Resets text color to default
     */
    void resetColor() const;
    
    /**
     * Sets text color using platform-specific implementations
     */
    void setColor(int foreground, int background) const;
    
    /**
     * Cross-platform implementation of sleep/delay function
     */
    void sleep(int milliseconds) const;
}; 