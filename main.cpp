#include "Headers.h"
#include <iomanip>
#include <windows.h>
#include <fcntl.h>
#include <io.h>

// Function prototypes for helper functions
void displayMenu();
void setConsoleTitle();
void setConsoleSize();
void displayIntro();
void configureConsole();

int main() {
    // Configure console for proper Unicode display
    configureConsole();
    
    // Set up the console for better appearance
    setConsoleTitle();
    setConsoleSize();
    
    // Display the intro
    displayIntro();
    
    // Create the game instance
    Game game;
    
    // Display the main menu
    displayMenu();
    
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

// Configure console for proper Unicode display
void configureConsole() {
    // Set console output code page to UTF-8
    SetConsoleOutputCP(CP_UTF8);
    
    // Set console input code page to UTF-8
    SetConsoleCP(CP_UTF8);
    
    // Set console mode to process virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    
    // Change console font to Consolas or another Unicode-compatible font
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;  // Width of each character
    cfi.dwFontSize.Y = 16; // Height of each character
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, L"Consolas"); // A good Unicode font
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}

// Sets the console window title
void setConsoleTitle() {
    SetConsoleTitle(TEXT("Chess Game"));
}

// Sets the console window size
void setConsoleSize() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = {0, 0, 79, 29}; // Width: 80, Height: 30
    SetConsoleWindowInfo(console, TRUE, &windowSize);
}

// Displays the intro screen
void displayIntro() {
    system("cls");
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Display logo with a blue color scheme
    SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::cout << std::endl;
    std::cout << "   ██████╗██╗  ██╗███████╗███████╗███████╗" << std::endl;
    std::cout << "  ██╔════╝██║  ██║██╔════╝██╔════╝██╔════╝" << std::endl;
    std::cout << "  ██║     ███████║█████╗  ███████╗███████╗" << std::endl;
    std::cout << "  ██║     ██╔══██║██╔══╝  ╚════██║╚════██║" << std::endl;
    std::cout << "  ╚██████╗██║  ██║███████╗███████║███████║" << std::endl;
    std::cout << "   ╚═════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝" << std::endl;
    
    // Display title
    SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::cout << std::setw(47) << "Classic Chess Game" << std::endl;
    
    // Display version and author
    SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::cout << std::setw(46) << "Version 1.0" << std::endl;
    std::cout << std::endl;
    
    // Pause to show intro screen
    Sleep(1500);
}

// Displays the main menu
void displayMenu() {
    system("cls");
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Display title
    SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::cout << std::endl;
    std::cout << std::setw(47) << "CHESS GAME" << std::endl;
    std::cout << std::setw(47) << "----------" << std::endl;
    std::cout << std::endl;
    
    // Display menu options
    SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::cout << "  Please select your preferred input method:" << std::endl;
    std::cout << std::endl;
    
    // Option 1 - Mouse input
    SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << "  1. Mouse Input" << std::endl;
    SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::cout << "     • Click on a piece to select it" << std::endl;
    std::cout << "     • Click on a destination square to move" << std::endl;
    std::cout << "     • Recommended for most players" << std::endl;
    std::cout << std::endl;
    
    // Option 2 - Text input
    SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << "  2. Text Input" << std::endl;
    SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::cout << "     • Enter moves using algebraic notation (e.g., 'e2e4')" << std::endl;
    std::cout << "     • First two characters represent the source square" << std::endl;
    std::cout << "     • Last two characters represent the destination square" << std::endl;
    std::cout << "     • For experienced players or keyboard enthusiasts" << std::endl;
    std::cout << std::endl;
}