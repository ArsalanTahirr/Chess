#include "../include/Headers.h"
#include "../include/ui/ChessUI.h"
#include <iomanip>

// Platform-specific includes
#ifdef _WIN32
#include <windows.h>  // Windows API for console handling
#include <fcntl.h>    // File control options
#include <io.h>       // Low-level I/O
#else
#include <unistd.h>   // POSIX operating system API
#endif

/**
 * Main entry point for the Chess application
 * 
 * Initializes the UI and game components, displays the menu,
 * handles input mode selection, and starts the game loop.
 */
int main() {
    // Initialize the UI system with cross-platform support
    ChessUI ui;
    
    // Configure console appearance
    ui.setConsoleTitle();
    ui.setConsoleSize();
    
    // Show the application intro screen
    ui.displayIntro();
    
    // Initialize the chess game engine
    Game game;
    
    // Display main menu with input options
    ui.displayMainMenu();
    
    // Get user's preferred input method (mouse or text)
    char inputMode;
    std::cout << "  Select option (1-2): ";
    std::cin >> inputMode;
    std::cin.clear();           // Clear any input errors
    std::cin.ignore(9999, '\n'); // Clear the input buffer
    
    // Configure the game based on selected input mode
    if (inputMode == '2') {
        game.setMouseInputMode(false); // Use text-based algebraic notation input
    } else if (inputMode != '1') {
        // Default to mouse input for invalid selections
        std::cout << "  Invalid option, defaulting to mouse input." << std::endl;
        std::cout << "  Press Enter to continue...";
        std::cin.get();
    }
    
    // Initialize the game state and start the main game loop
    game.startGame();  // Set up the board, pieces, and players
    game.playGame();   // Begin the turn-based gameplay loop
    
    return 0;
}
