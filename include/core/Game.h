#pragma once

#include "StateString.h"
#include <vector>

// Forward declarations
class Board;
class Player;
class InputParser;
class Move;
class ChessUI;
class Piece;

class Game {
private:
    Board* board;
    Player* whitePlayer;
    Player* blackPlayer;
    Player* currentPlayer;
    InputParser* inputParser;
    ChessUI* ui;
    std::vector<StateString> moveHistory;
    std::string lastErrorMessage;
    bool hasError;
    bool useMouseInput;
public:
    Game();
    ~Game();
    void startGame();
    void playGame();
    void makeMove(Move& move);
    void resetEnpassantAbility(Piece* piece);
    void setError(const std::string& error);
    void clearError();
    void setMouseInputMode(bool useMouseInput);
};

