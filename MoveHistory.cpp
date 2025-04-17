#include <iostream>
#include "MoveHistory.h"

MoveHistory::MoveHistory() {}

MoveHistory::~MoveHistory() {}

void MoveHistory::addMove(Move* move) {
    moves.push_back(move);
}

Move* MoveHistory::getLastMove() const {
    if(moves.empty()) {
        return nullptr;
    }
    return moves.back();
}

std::vector<Move*> MoveHistory::getAllMoves() const { return moves; }

void MoveHistory::undoLastMove() {
    if(!moves.empty()) {
        moves.pop_back();
    }
}