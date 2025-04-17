#include <iostream>
#include "Piece.h"
#include "Player.h"

Player::Player(Color color, const std::string& name): color(color), name(name) {}

Player::~Player() {}

Color Player::getColor() const { return color; }

std::string Player::getName() const { return name; }

void Player::addCapturedPiece(Piece* piece) { 
    capturedPieces.push_back(piece); 
    piece->setisCaptured(true); 
}

std::vector<Piece*> Player::getCapturedPieces() const { return capturedPieces; }