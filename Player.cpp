#include <iostream>
#include "Piece.h"
#include "Player.h"

Player::Player(const std::string& name, Color color): name(name), color(color) {}

Player::~Player() {
    for(Piece* piece : capturedPieces) {
        delete piece;
    }
}

Color Player::getColor() const { return color; }

std::string Player::getName() const { return name; }

void Player::addCapturedPiece(Piece* piece) { 
    capturedPieces.push_back(piece); 
    piece->setisCaptured(true); 
}

std::vector<Piece*> Player::getCapturedPieces() const { return capturedPieces; }