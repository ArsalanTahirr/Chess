#include <iostream>
#include <windows.h>
#include "GlobalVariables.h"

const int BOARD_ROWS = 8;

const int BOARD_COLS = 8;

// Define colors
const int BLACK_SQUARE_BG = BACKGROUND_RED | BACKGROUND_GREEN;          // Brown (red + green)
const int WHITE_SQUARE_BG = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY; // Light brown
const int BLACK_PIECE_FG = 0;                                           // Black
const int WHITE_PIECE_FG = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; // Bright white

// Define square size
const int SQUARE_WIDTH = 8;  // Width of each square in characters
const int SQUARE_HEIGHT = 3; // Height of each square in rows