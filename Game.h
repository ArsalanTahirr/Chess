#pragma once

#include "StateString.h"
#include <vector>
class Board;
class Player;
class InputParser;
class Move;

class Game {
private:
    Board* board;
    Player* whitePlayer;
    Player* blackPlayer;
    Player* currentPlayer;
    InputParser* inputParser;
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
