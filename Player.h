#pragma once

#include "Enums.h"
#include <vector>
#include <string>

class Piece;

class Player {
private:
    Color color;
    std::string name;
    std::vector<Piece*> capturedPieces;
public:
    Player(Color color, const std::string& name);
    ~Player();
    Color getColor() const;
    std::string getName() const;
    void addCapturedPiece(Piece* piece);
    std::vector<Piece*> getCapturedPieces() const;
};