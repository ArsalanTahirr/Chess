#pragma once

#include "Enums.h"
#include <vector>
#include <string>

class Piece;

class Player {
private:
    std::string name;
    Color color;
    std::vector<Piece*> capturedPieces;
public:
    Player(const std::string& name, Color color);
    ~Player();
    Color getColor() const;
    std::string getName() const;
    void addCapturedPiece(Piece* piece);
    std::vector<Piece*> getCapturedPieces() const;
};