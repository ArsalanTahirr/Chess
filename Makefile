# Detect operating system
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
    RM_CMD := powershell -Command "if (Test-Path $(1)) { Remove-Item $(1) }"
    MKDIR_CMD := powershell -Command "if (-not (Test-Path $(1))) { New-Item -ItemType Directory -Path $(1) }"
    PATH_SEP := \\
    EXE_EXT := .exe
    RUN_PREFIX := 
    # Windows doesn't support ANSI colors in cmd.exe by default
    # But PowerShell and Windows Terminal do
    COLOR_RED :=
    COLOR_GREEN :=
    COLOR_YELLOW :=
    COLOR_BLUE :=
    COLOR_RESET :=
else
    DETECTED_OS := $(shell uname -s)
    RM_CMD := rm -f $(1)
    MKDIR_CMD := mkdir -p $(1)
    PATH_SEP := /
    EXE_EXT := 
    RUN_PREFIX := ./
    # ANSI color codes for Unix terminals
    COLOR_RED := \033[1;31m
    COLOR_GREEN := \033[1;32m
    COLOR_YELLOW := \033[1;33m
    COLOR_BLUE := \033[1;34m
    COLOR_RESET := \033[0m
endif

# Define a space variable for use in substitutions
empty :=
space := $(empty) $(empty)

# Check for verbose mode (use make V=1 for verbose output)
ifeq ($(V),1)
    VERBOSE := 1
    Q :=
else
    VERBOSE := 0
    Q := @
endif

# Compiler to use
CC = g++

# Compiler flags
CFLAGS = -std=c++17 -Wall

# Add flags for better error reporting (if supported)
ifneq ($(DETECTED_OS),Windows)
    CFLAGS += -fmacro-prefix-map=./=
endif

# Target executable name
TARGET = chess$(EXE_EXT)

# Include directory
INCLUDE_DIR = include

# Source directories with forward slashes (works with MinGW)
SRC_DIR = src
SRC_CORE = $(SRC_DIR)/core
SRC_PIECES = $(SRC_DIR)/pieces
SRC_UTILS = $(SRC_DIR)/utils
SRC_UI = $(SRC_DIR)/ui

# Find all source files (use forward slashes for GCC compatibility)
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) \
          $(wildcard $(SRC_CORE)/*.cpp) \
          $(wildcard $(SRC_PIECES)/*.cpp) \
          $(wildcard $(SRC_UTILS)/*.cpp) \
          $(wildcard $(SRC_UI)/*.cpp)

# Object files (maintain directory structure)
OBJECTS = $(SOURCES:.cpp=.o)

# Function to get relative path for display
define get_relative_path
$(patsubst $(CURDIR)/%,%,$(abspath $(1)))
endef

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
ifeq ($(DETECTED_OS),Windows)
	@echo Linking $(TARGET)...
else
	@printf "$(COLOR_GREEN)Linking$(COLOR_RESET) $(TARGET)...\n"
endif
ifeq ($(VERBOSE),1)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)
else
	$(Q)$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)
endif
ifeq ($(DETECTED_OS),Windows)
	@echo Build complete: $(TARGET)
else
	@printf "$(COLOR_GREEN)Build complete:$(COLOR_RESET) $(TARGET)\n"
endif

# Compile source files to object files (OS-specific implementations)
ifeq ($(DETECTED_OS),Windows)
%.o: %.cpp
	@echo Compiling $(call get_relative_path,$<)...
ifeq ($(VERBOSE),1)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@ -E > $*.i || (echo "Error in $(call get_relative_path,$<)" && exit 1)
	@echo Preprocessor defines used:
	@findstr /C:"#define" $*.i | findstr /V /C:"#define __" | findstr /V /C:"#define _" | head -5
	@del $*.i 2>NUL
else
	$(Q)$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@ || (echo "Error in $(call get_relative_path,$<)" && exit 1)
endif
else
%.o: %.cpp
	@printf "$(COLOR_BLUE)Compiling$(COLOR_RESET) $(call get_relative_path,$<)...\n"
ifeq ($(VERBOSE),1)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@ -H 2>/tmp/guards_$*.txt || (printf "$(COLOR_RED)Error in $(call get_relative_path,$<)$(COLOR_RESET)\n" && $(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@)
else
	$(Q)$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@ -H 2>/tmp/guards_$*.txt || (printf "$(COLOR_RED)Error in $(call get_relative_path,$<)$(COLOR_RESET)\n" && $(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@)
endif
	@if [ -f /tmp/guards_$*.txt ]; then grep -E "^\." /tmp/guards_$*.txt | head -5 | sed 's/^\.//g' | sed 's/^ *//g' | sed 's/^/  Include: /g'; rm /tmp/guards_$*.txt; fi
endif

# Clean up (OS-independent)
clean:
ifeq ($(DETECTED_OS),Windows)
	@echo Cleaning object files and executable...
	$(Q)powershell -Command "if (Test-Path *.o) { Remove-Item *.o }"
	$(Q)powershell -Command "if (Test-Path $(subst /,\\,$(SRC_DIR))\\*.o) { Remove-Item $(subst /,\\,$(SRC_DIR))\\*.o }"
	$(Q)powershell -Command "if (Test-Path $(subst /,\\,$(SRC_CORE))\\*.o) { Remove-Item $(subst /,\\,$(SRC_CORE))\\*.o }"
	$(Q)powershell -Command "if (Test-Path $(subst /,\\,$(SRC_PIECES))\\*.o) { Remove-Item $(subst /,\\,$(SRC_PIECES))\\*.o }"
	$(Q)powershell -Command "if (Test-Path $(subst /,\\,$(SRC_UTILS))\\*.o) { Remove-Item $(subst /,\\,$(SRC_UTILS))\\*.o }"
	$(Q)powershell -Command "if (Test-Path $(subst /,\\,$(SRC_UI))\\*.o) { Remove-Item $(subst /,\\,$(SRC_UI))\\*.o }"
	$(Q)powershell -Command "if (Test-Path $(TARGET)) { Remove-Item $(TARGET) }"
	@echo Clean complete
else
	@printf "$(COLOR_YELLOW)Cleaning$(COLOR_RESET) object files and executable...\n"
	$(Q)rm -f *.o
	$(Q)rm -f $(SRC_DIR)/*.o
	$(Q)rm -f $(SRC_CORE)/*.o
	$(Q)rm -f $(SRC_PIECES)/*.o
	$(Q)rm -f $(SRC_UTILS)/*.o
	$(Q)rm -f $(SRC_UI)/*.o
	$(Q)rm -f $(TARGET)
	@printf "$(COLOR_GREEN)Clean complete$(COLOR_RESET)\n"
endif

# Run the game
run: $(TARGET)
ifeq ($(DETECTED_OS),Windows)
	@echo Running $(TARGET)...
else
	@printf "$(COLOR_GREEN)Running$(COLOR_RESET) $(TARGET)...\n"
endif
	$(Q)$(RUN_PREFIX)$(TARGET)

# Create directories if they don't exist
dirs:
ifeq ($(DETECTED_OS),Windows)
	@echo Creating directory structure...
else
	@printf "$(COLOR_BLUE)Creating$(COLOR_RESET) directory structure...\n"
endif
ifeq ($(DETECTED_OS),Windows)
	$(Q)$(call MKDIR_CMD,$(subst /,\\,$(SRC_CORE)))
	$(Q)$(call MKDIR_CMD,$(subst /,\\,$(SRC_PIECES)))
	$(Q)$(call MKDIR_CMD,$(subst /,\\,$(SRC_UTILS)))
	$(Q)$(call MKDIR_CMD,$(subst /,\\,$(SRC_UI)))
	$(Q)$(call MKDIR_CMD,$(subst /,\\,$(INCLUDE_DIR))\\core)
	$(Q)$(call MKDIR_CMD,$(subst /,\\,$(INCLUDE_DIR))\\pieces)
	$(Q)$(call MKDIR_CMD,$(subst /,\\,$(INCLUDE_DIR))\\utils)
	$(Q)$(call MKDIR_CMD,$(subst /,\\,$(INCLUDE_DIR))\\ui)
else
	$(Q)$(call MKDIR_CMD,$(SRC_CORE))
	$(Q)$(call MKDIR_CMD,$(SRC_PIECES))
	$(Q)$(call MKDIR_CMD,$(SRC_UTILS))
	$(Q)$(call MKDIR_CMD,$(SRC_UI))
	$(Q)$(call MKDIR_CMD,$(INCLUDE_DIR)/core)
	$(Q)$(call MKDIR_CMD,$(INCLUDE_DIR)/pieces)
	$(Q)$(call MKDIR_CMD,$(INCLUDE_DIR)/utils)
	$(Q)$(call MKDIR_CMD,$(INCLUDE_DIR)/ui)
endif
ifeq ($(DETECTED_OS),Windows)
	@echo Directory structure created
else
	@printf "$(COLOR_GREEN)Directory structure created$(COLOR_RESET)\n"
endif

# Print OS information (for debugging)
os-info:
ifeq ($(DETECTED_OS),Windows)
	@echo "Detected OS: $(DETECTED_OS)"
	@echo "Path separator: $(PATH_SEP)"
	@echo "Executable extension: $(EXE_EXT)"
else
	@printf "$(COLOR_BLUE)Detected OS:$(COLOR_RESET) $(DETECTED_OS)\n"
	@printf "$(COLOR_BLUE)Path separator:$(COLOR_RESET) $(PATH_SEP)\n"
	@printf "$(COLOR_BLUE)Executable extension:$(COLOR_RESET) $(EXE_EXT)\n"
endif

# Show all source files that will be compiled (simplified)
show-sources:
	@echo Source files hierarchy:
	@echo   src/
	@echo     main.cpp
	@echo     core/
	@echo       Board.cpp
	@echo       ChessRules.cpp
	@echo       Game.cpp
	@echo       Move.cpp
	@echo       Player.cpp
	@echo       Position.cpp
	@echo       Square.cpp
	@echo       StateString.cpp
	@echo     pieces/
	@echo       Bishop.cpp
	@echo       King.cpp
	@echo       Knight.cpp
	@echo       Pawn.cpp
	@echo       Piece.cpp
	@echo       Queen.cpp
	@echo       Rook.cpp
	@echo     ui/
	@echo       ChessUI.cpp
	@echo       InputParser.cpp
	@echo     utils/
	@echo       GlobalVariables.cpp

# Show all header files (simplified)
show-headers:
	@echo Header files hierarchy:
	@echo   include/
	@echo     Enums.h
	@echo     Headers.h
	@echo     core/
	@echo       Board.h
	@echo       ChessRules.h
	@echo       Game.h
	@echo       Move.h
	@echo       Player.h
	@echo       Position.h
	@echo       Square.h
	@echo       StateString.h
	@echo     pieces/
	@echo       Bishop.h
	@echo       King.h
	@echo       Knight.h
	@echo       Pawn.h
	@echo       Piece.h
	@echo       Queen.h
	@echo       Rook.h
	@echo     ui/
	@echo       ChessUI.h
	@echo       InputParser.h
	@echo     utils/
	@echo       GlobalVariables.h

# Show project structure (simplified)
show-structure:
	@echo Project structure:
	@echo Chess/
	@echo   include/
	@echo     core/
	@echo     pieces/
	@echo     ui/
	@echo     utils/
	@echo   src/
	@echo     core/
	@echo     pieces/
	@echo     ui/
	@echo     utils/
	@echo   Other files:
	@echo     Makefile
	@echo     README.md
	@echo     chess.exe

# Help target
help:
ifeq ($(DETECTED_OS),Windows)
	@echo "Chess Game Makefile Help"
	@echo "-----------------------"
	@echo "Available targets:"
	@echo "  all          - Build the chess game (default)"
	@echo "  clean        - Remove object files and executable"
	@echo "  run          - Build and run the chess game"
	@echo "  dirs         - Create directory structure"
	@echo "  os-info      - Display OS information"
	@echo "  show-sources - List all source files"
	@echo "  show-headers - List all header files"
	@echo "  show-structure - List project structure"
	@echo "  help         - Display this help message"
	@echo ""
	@echo "Options:"
	@echo "  V=1          - Enable verbose output (shows full commands)"
else
	@printf "$(COLOR_YELLOW)Chess Game Makefile Help$(COLOR_RESET)\n"
	@printf "$(COLOR_YELLOW)-----------------------$(COLOR_RESET)\n"
	@printf "Available targets:\n"
	@printf "  $(COLOR_GREEN)all$(COLOR_RESET)          - Build the chess game (default)\n"
	@printf "  $(COLOR_GREEN)clean$(COLOR_RESET)        - Remove object files and executable\n"
	@printf "  $(COLOR_GREEN)run$(COLOR_RESET)          - Build and run the chess game\n"
	@printf "  $(COLOR_GREEN)dirs$(COLOR_RESET)         - Create directory structure\n"
	@printf "  $(COLOR_GREEN)os-info$(COLOR_RESET)      - Display OS information\n"
	@printf "  $(COLOR_GREEN)show-sources$(COLOR_RESET) - List all source files\n"
	@printf "  $(COLOR_GREEN)show-headers$(COLOR_RESET) - List all header files\n"
	@printf "  $(COLOR_GREEN)show-structure$(COLOR_RESET) - List project structure\n"
	@printf "  $(COLOR_GREEN)help$(COLOR_RESET)         - Display this help message\n"
	@printf "\n"
	@printf "Options:\n"
	@printf "  $(COLOR_BLUE)V=1$(COLOR_RESET)          - Enable verbose output (shows full commands)\n"
endif