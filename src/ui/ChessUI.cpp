#include "../../include/ui/ChessUI.h"
#include "../../include/core/Board.h"
#include "../../include/core/Player.h"
#include "../../include/utils/GlobalVariables.h"
#include <iostream>
#include <iomanip>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#else
#include <chrono>
#include <thread>
#endif

ChessUI::ChessUI() {
#ifdef _WIN32
    console = GetStdHandle(STD_OUTPUT_HANDLE);
    useColor = true;
#else
    // Check if terminal supports colors
    const char* term = getenv("TERM");
    useColor = term && (
        strcmp(term, "xterm") == 0 ||
        strcmp(term, "xterm-color") == 0 ||
        strcmp(term, "xterm-256color") == 0 ||
        strcmp(term, "screen") == 0 ||
        strcmp(term, "screen-256color") == 0 ||
        strcmp(term, "tmux") == 0 ||
        strcmp(term, "tmux-256color") == 0 ||
        strcmp(term, "rxvt") == 0 ||
        strcmp(term, "rxvt-unicode") == 0 ||
        strcmp(term, "linux") == 0 ||
        strcmp(term, "cygwin") == 0
    );
#endif
    configureConsole();
}

ChessUI::~ChessUI() {
    // No dynamic resources to clean up
}

void ChessUI::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    // ANSI escape code to clear screen
    std::cout << "\033[2J\033[1;1H";
#endif
}

void ChessUI::displayChessLogo() const {
#ifdef _WIN32
    if (useColor) {
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }
#else
    if (useColor) {
        std::cout << "\033[1;37m"; // Bright white
    }
#endif
    
    std::cout << std::endl;
    std::cout << "   ██████╗██╗  ██╗███████╗███████╗███████╗" << std::endl;
    std::cout << "  ██╔════╝██║  ██║██╔════╝██╔════╝██╔════╝" << std::endl;
    std::cout << "  ██║     ███████║█████╗  ███████╗███████╗" << std::endl;
    std::cout << "  ██║     ██╔══██║██╔══╝  ╚════██║╚════██║" << std::endl;
    std::cout << "  ╚██████╗██║  ██║███████╗███████║███████║" << std::endl;
    std::cout << "   ╚═════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝" << std::endl;
    std::cout << std::endl;
    
    resetColor();
}

void ChessUI::displayCenteredMessage(const std::string& message, bool highlight) const {
    if (highlight && useColor) {
#ifdef _WIN32
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
        std::cout << "\033[1;33m"; // Bright yellow
#endif
    }
    
    std::cout << std::setw(40 + message.length()/2) << message << std::endl;
    
    if (highlight) {
        resetColor();
    }
}

void ChessUI::displayWelcomeScreen() const {
    clearScreen();
    displayChessLogo();
    displayCenteredMessage("Welcome to Chess", true);
    displayCenteredMessage("A classic two-player strategy game");
    std::cout << std::endl;
}

void ChessUI::displayGameInstructions(const std::string& whitePlayerName, const std::string& blackPlayerName, bool useMouseInput) const {
    clearScreen();
    displayChessLogo();
    displayCenteredMessage("Game Instructions", true);
    std::cout << std::endl;
    std::cout << " • White pieces (" << whitePlayerName << ") move first" << std::endl;
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
}

void ChessUI::displayMainMenu() const {
    clearScreen();
    
    // Display title
    if (useColor) {
#ifdef _WIN32
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#else
        std::cout << "\033[1;37m"; // Bright white
#endif
    }
    
    std::cout << std::endl;
    std::cout << std::setw(47) << "CHESS GAME" << std::endl;
    std::cout << std::setw(47) << "----------" << std::endl;
    std::cout << std::endl;
    
    // Display menu options
    resetColor();
    std::cout << "  Please select your preferred input method:" << std::endl;
    std::cout << std::endl;
    
    // Option 1 - Mouse input
    if (useColor) {
#ifdef _WIN32
        SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
        std::cout << "\033[1;32m"; // Bright green
#endif
    }
    
    std::cout << "  1. Mouse Input" << std::endl;
    resetColor();
    std::cout << "     • Click on a piece to select it" << std::endl;
    std::cout << "     • Click on a destination square to move" << std::endl;
    std::cout << "     • Recommended for most players" << std::endl;
    std::cout << std::endl;
    
    // Option 2 - Text input
    if (useColor) {
#ifdef _WIN32
        SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
        std::cout << "\033[1;32m"; // Bright green
#endif
    }
    
    std::cout << "  2. Text Input" << std::endl;
    resetColor();
    std::cout << "     • Enter moves using algebraic notation (e.g., 'e2e4')" << std::endl;
    std::cout << "     • First two characters represent the source square" << std::endl;
    std::cout << "     • Last two characters represent the destination square" << std::endl;
    std::cout << "     • For experienced players or keyboard enthusiasts" << std::endl;
    std::cout << std::endl;
}

void ChessUI::displayIntro() const {
    clearScreen();
    
    // Display logo with a blue color scheme
    if (useColor) {
#ifdef _WIN32
        SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#else
        std::cout << "\033[1;34m"; // Bright blue
#endif
    }
    
    std::cout << std::endl;
    std::cout << "   ██████╗██╗  ██╗███████╗███████╗███████╗" << std::endl;
    std::cout << "  ██╔════╝██║  ██║██╔════╝██╔════╝██╔════╝" << std::endl;
    std::cout << "  ██║     ███████║█████╗  ███████╗███████╗" << std::endl;
    std::cout << "  ██║     ██╔══██║██╔══╝  ╚════██║╚════██║" << std::endl;
    std::cout << "  ╚██████╗██║  ██║███████╗███████║███████║" << std::endl;
    std::cout << "   ╚═════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝" << std::endl;
    
    // Display title
    if (useColor) {
#ifdef _WIN32
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#else
        std::cout << "\033[1;37m"; // Bright white
#endif
    }
    
    std::cout << std::setw(47) << "Classic Chess Game" << std::endl;
    
    // Display version and author
    resetColor();
    std::cout << std::setw(46) << "Version 1.0" << std::endl;
    std::cout << std::endl;
    
    // Pause to show intro screen
    sleep(1500);
}

void ChessUI::displayBoard(const Board* board) const {
    if (!board) return;
    
    board->printBoard();
}

void ChessUI::displayCurrentPlayerInfo(const Player* player, bool isInCheck) const {
    if (!player) return;
    
    if (useColor) {
        if (player->getColor() == Color::White) {
#ifdef _WIN32
            SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#else
            std::cout << "\033[1;37m"; // Bright white
#endif
        } else {
#ifdef _WIN32
            SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
#else
            std::cout << "\033[1;30m"; // Bright black (gray)
#endif
        }
    }
    
    std::cout << std::endl << " " << player->getName() << "'s turn (";
    std::cout << (player->getColor() == Color::White ? "White" : "Black") << ")" << std::endl;
    
    if (isInCheck && useColor) {
#ifdef _WIN32
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
        std::cout << "\033[1;31m"; // Bright red
#endif
        std::cout << " YOUR KING IS IN CHECK! You must address this threat." << std::endl;
    }
    
    resetColor();
}

void ChessUI::displayErrorMessage(const std::string& errorMessage) const {
    if (useColor) {
#ifdef _WIN32
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
        std::cout << "\033[1;31m"; // Bright red
#endif
    }
    
    std::cout << " Error: " << errorMessage << std::endl;
    resetColor();
}

void ChessUI::displayMovePrompt(bool useMouseInput) const {
    if (useMouseInput) {
        std::cout << " Please click on a piece to move..." << std::endl;
    } else {
        std::cout << " Enter your move: ";
    }
}

void ChessUI::displayGameOver(const std::string& message) const {
    std::cout << std::endl;
    std::cout << message << std::endl;
    std::cout << "Game over! Press Enter to exit...";
}

void ChessUI::displayCheckmate(const Player* winner, const Player* loser) const {
    if (!winner || !loser) return;
    
    if (useColor) {
#ifdef _WIN32
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
        std::cout << "\033[1;31m"; // Bright red
#endif
    }
    
    std::cout << std::endl;
    std::cout << " ♚ CHECKMATE! ♚" << std::endl;
    resetColor();
    
    std::cout << " " << loser->getName() << " is in checkmate!" << std::endl;
    
    // Display winner
    if (useColor) {
#ifdef _WIN32
        SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
        std::cout << "\033[1;32m"; // Bright green
#endif
    }
    
    std::cout << " " << winner->getName() << " wins the game!" << std::endl;
    resetColor();
}

void ChessUI::displayStalemate(const Player* player) const {
    if (!player) return;
    
    if (useColor) {
#ifdef _WIN32
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
        std::cout << "\033[1;33m"; // Bright yellow
#endif
    }
    
    std::cout << std::endl;
    std::cout << " ♖ STALEMATE! ♖" << std::endl;
    resetColor();
    
    std::cout << " " << player->getName() << " has no legal moves!" << std::endl;
    std::cout << " The game is a draw!" << std::endl;
}

void ChessUI::displayDraw(const std::string& reason) const {
    if (useColor) {
#ifdef _WIN32
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
        std::cout << "\033[1;33m"; // Bright yellow
#endif
    }
    
    std::cout << std::endl;
    std::cout << " ♘ DRAW! ♘" << std::endl;
    resetColor();
    
    std::cout << " " << reason << std::endl;
}

void ChessUI::configureConsole() const {
#ifdef _WIN32
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
#else
    // Enable UTF-8 output
    std::cout << "\033[0m"; // Reset all attributes
    // No special configuration needed for most Unix terminals
#endif
}

void ChessUI::setConsoleTitle() const {
#ifdef _WIN32
    SetConsoleTitle(TEXT("Chess Game"));
#else
    // Set terminal title using ANSI escape sequence
    std::cout << "\033]0;Chess Game\007";
#endif
}

void ChessUI::setConsoleSize() const {
#ifdef _WIN32
    SMALL_RECT windowSize = {0, 0, 79, 29}; // Width: 80, Height: 30
    SetConsoleWindowInfo(console, TRUE, &windowSize);
#else
    // Terminal size can't be reliably set in all Unix terminals
    // Could use ncurses for more control if needed
#endif
}

void ChessUI::resetColor() const {
    if (useColor) {
#ifdef _WIN32
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
        std::cout << "\033[0m"; // Reset to default
#endif
    }
}

void ChessUI::setColor(int foreground, int background) const {
    if (useColor) {
#ifdef _WIN32
        SetConsoleTextAttribute(console, foreground | background);
#else
        // Convert Windows color codes to ANSI (simplified)
        std::cout << "\033[" << (foreground > 7 ? "1;" : "0;") << (30 + (foreground & 7)) << "m";
        if (background > 0) {
            std::cout << "\033[" << (40 + (background & 7)) << "m";
        }
#endif
    }
}

void ChessUI::sleep(int milliseconds) const {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
#endif
} 