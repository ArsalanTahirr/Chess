# Chess Game

A cross-platform chess game implementation in C++ featuring a clean architecture with separation of concerns. This console-based chess application provides a complete implementation of chess rules with both mouse and keyboard input support.

## Screenshots

### Loading Screen
![Loading Screen](images/ScreenShot_1.png)

### Welcome Screen
![Welcome Screen](images/ScreenShot_2.png)

### Game Instructions
![Game Instructions](images/ScreenShot_3.png)

### Chess Board
![Chess Board](images/ScreenShot_4.png)

## Features

- Complete chess rules implementation
- Cross-platform compatibility (Windows and Unix-based systems)
- Mouse and keyboard input options
- Unicode chess piece symbols with color support
- Castling, en passant, and pawn promotion
- Check and checkmate detection
- Stalemate and draw detection
- Captured pieces tracking
- Intuitive user interface with color-coded messages
- Comprehensive error handling

## Architecture

The project follows a modular architecture with clear separation of concerns:

### Core Components

- **Game Logic**: Implements chess rules, move validation, and game state management
- **Board Representation**: Manages the chess board state and piece positions
- **Piece Hierarchy**: Object-oriented design for different chess pieces with polymorphic behavior
- **UI Abstraction**: Platform-independent UI layer that separates rendering from game logic

### Design Patterns

- **Factory Pattern**: For creating different chess pieces
- **Strategy Pattern**: For piece movement strategies
- **Singleton Pattern**: For global game state management
- **Observer Pattern**: For game state updates and UI notifications

### Directory Structure

```
Chess/
├── include/              # Header files
│   ├── core/             # Core game logic headers
│   ├── pieces/           # Chess piece class headers
│   ├── utils/            # Utility and helper headers
│   └── ui/               # UI abstraction layer headers
├── src/                  # Implementation files
│   ├── core/             # Core game logic implementation
│   ├── pieces/           # Chess piece implementations
│   ├── utils/            # Utility implementations
│   └── ui/               # UI implementations
├── images/               # Screenshot images
├── Makefile              # Build configuration
└── README.md             # Project documentation
```

## Building and Running

### Prerequisites

- C++17 compatible compiler (GCC, Clang, or MSVC)
- Make build system

### Build Commands

Build the game:
```
make
```

Run the game:
```
make run
```

Clean build artifacts:
```
make clean
```

Show build information:
```
make os-info
```

Create directory structure (if needed):
```
make dirs
```

Display verbose compilation output:
```
make V=1
```

### Additional Makefile Commands

View project structure:
```
make show-structure
```

List source files:
```
make show-sources
```

List header files:
```
make show-headers
```

Display help:
```
make help
```

## Cross-Platform Support

The game is designed to work seamlessly on both Windows and Unix-based systems:

- **Windows**: Supports command prompt and PowerShell with Unicode chess symbols
- **Unix/Linux/macOS**: Supports terminal with ANSI color codes and Unicode

The codebase uses conditional compilation with platform-specific implementations:
- Console handling (`#ifdef _WIN32`)
- Color support
- Screen clearing
- Input handling

## How to Play

The game supports two input methods:

### Mouse Input
- Click on a piece to select it
- Click on a destination square to move
- Selected pieces are highlighted
- Valid move destinations are indicated

### Text Input
- Enter moves in algebraic notation (e.g., 'e2e4')
- Type 'o-o' for kingside castling
- Type 'o-o-o' for queenside castling
- For pawn promotion, use format 'e7e8=Q' (replace Q with desired piece)
- Type 'quit' or 'exit' to end the game

## UI Abstraction Layer

The `ChessUI` class provides a clean abstraction between game logic and rendering:

- **Platform Independence**: Handles platform-specific console operations
- **Consistent Interface**: Provides a unified API for the game engine
- **Separation of Concerns**: Isolates UI code from game logic
- **Enhanced Maintainability**: Makes it easier to modify UI without affecting game rules

Key UI methods:
- Board rendering with Unicode chess symbols
- Color-coded message display
- Game state visualization
- Input handling for both mouse and keyboard
- Cross-platform terminal configuration

## Technical Implementation

### Chess Rules Implementation

The game implements all standard chess rules including:

- Piece movement validation
- Check and checkmate detection
- Castling with proper validation
- En passant captures
- Pawn promotion
- Stalemate and draw conditions
- Fifty-move rule
- Threefold repetition

### Cross-Platform Compatibility

The application achieves cross-platform compatibility through:

- Conditional compilation with preprocessor directives
- Platform-specific implementations wrapped in common interfaces
- Unicode support for chess symbols
- ANSI color codes for Unix terminals
- Windows Console API for Windows systems

## Contributors

This project was developed by Computer Science students at FAST University Karachi:

- **Arsalan Tahir**
- **Wajiha Hassan**
- **Narmeen Zehra**

## License

This project is open source and available under the MIT License.

## Acknowledgments

Special thanks to our instructors at FAST University for their guidance and support throughout the development of this project.
