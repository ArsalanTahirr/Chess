#pragma once

#include <string>
#include <windows.h>
#include "Position.h"

class Move;
class Board;
class Player;

class InputParser {
private:
    std::string inputString;
    Board* board;
    bool firstClick;
    Position fromPosition;
    Position toPosition;
    
    // Mouse input handling
    bool handleMouseInput(COORD clickPos, Position& position);
    COORD getConsoleClickPosition();
    Position translateClickToChessPosition(COORD clickPos);
    
public:
    InputParser(Board* board);
    ~InputParser();
    Move takeInput();
    Move parseInput();
    Move takeMouseInput();
};