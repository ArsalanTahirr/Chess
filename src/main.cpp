#include "../include/Headers.h"
#include "../include/ui/ChessUI.h"
#include <iomanip>
#include <windows.h>
#include <fcntl.h>
#include <io.h>

int main() {
    // Create UI instance
    ChessUI ui;
    
    // Set up the console for better appearance
    ui.setConsoleTitle();
    ui.setConsoleSize();
    
    // Display the intro
    ui.displayIntro();
    
    // Create the game instance
    Game game;
    
    // Display the main menu
    ui.displayMainMenu();
    
    // Configure input mode
    char inputMode;
    std::cout << "  Select option (1-2): ";
    std::cin >> inputMode;
    std::cin.clear(); // Clear any input errors
    std::cin.ignore(9999, '\n'); // Clear the input buffer
    
    // Set the input mode
    if (inputMode == '2') {
        game.setMouseInputMode(false); // Use text input
    } else if (inputMode != '1') {
        // Default to mouse input if invalid input
        std::cout << "  Invalid option, defaulting to mouse input." << std::endl;
        std::cout << "  Press Enter to continue...";
        std::cin.get();
    }
    
    // Start and play the game
    game.startGame();
    game.playGame();
    
    return 0;
}
