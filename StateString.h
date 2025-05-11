#pragma once

#include <string>

class Player;
class Board;

class StateString {
private:
    std::string stateString;
public:
    StateString(Player& player, Board* board);
    ~StateString();
    void AddRowData(int row, Board* board);
    void AddPiecePlacement(Board* board);
    void AddCurrentPlayer(Player& player);
    void AddCastlingRights(Board* board);
    void AddEnPassantRights(Board* board, Player& player);
    std::string getStateString() const;
};