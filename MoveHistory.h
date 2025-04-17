#pragma once

#include <vector>

class Move;

class MoveHistory {
private:
    std::vector<Move*> moves;
public:
    MoveHistory();
    ~MoveHistory();
    void addMove(Move* move);
    Move* getLastMove() const;
    std::vector<Move*> getAllMoves() const;
    void undoLastMove();
};