#pragma once

/**
 * Main header file that includes all necessary dependencies for the Chess application
 * This file is organized into sections for standard library includes, platform-specific
 * includes, and the Chess application's component headers.
 */

// Standard C++ library includes
#include <iostream>   // Input/output stream functionality
#include <vector>     // Dynamic array container
#include <string>     // String handling
#include <algorithm>  // Algorithms like sort, find, etc.
#include <cctype>     // Character handling functions
#include <utility>    // Utility components like pair, move, etc.

// Platform-specific includes
#ifdef _WIN32
#include <windows.h>  // Windows API for console handling
#endif

// Core chess engine components
#include "core/Board.h"       // Chess board representation
#include "core/Position.h"    // Position on the chess board
#include "core/Square.h"      // Individual square on the board
#include "core/Player.h"      // Player information and state
#include "core/Move.h"        // Chess move representation
#include "core/ChessRules.h"  // Rules enforcement
#include "core/StateString.h" // Game state serialization
#include "core/Game.h"        // Main game logic

// Chess pieces
#include "pieces/Piece.h"     // Base piece class
#include "pieces/Pawn.h"      // Pawn implementation
#include "pieces/Knight.h"    // Knight implementation
#include "pieces/Bishop.h"    // Bishop implementation
#include "pieces/Rook.h"      // Rook implementation
#include "pieces/Queen.h"     // Queen implementation
#include "pieces/King.h"      // King implementation

// Enumerations and constants
#include "Enums.h"            // Game enumerations (Color, PieceType, etc.)

// UI components
#include "ui/InputParser.h"   // Parse user input
#include "ui/ChessUI.h"       // UI rendering and interaction

// Utilities
#include "utils/GlobalVariables.h" // Global configuration and constants

// Use the standard namespace for convenience
using namespace std;