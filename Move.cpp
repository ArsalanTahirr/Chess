#include <iostream>
#include "Move.h"
#include "Piece.h"

Move::Move(): from(Position(-1, -1)), to(Position(-1, -1)), pieceMoved(nullptr), pieceCaptured(nullptr),
        isPromotion(false), isCastlingKingSide(false), isCastlingQueenSide(false),
        pieceType(PieceType::None) {}

Move::~Move() {}

Position Move::getFrom() const { return from; }

Position Move::getTo() const { return to; }

Piece* Move::getPieceMoved() const { return pieceMoved; }

PieceType Move::getPieceType() const { return pieceType; }

void Move::setPieceType(PieceType type) { pieceType = type; }

Piece* Move::getPieceCaptured() const { return pieceCaptured; }

bool Move::getIsPromotion() const { return isPromotion; }

bool Move::getIsCastlingKingSide() const { return isCastlingKingSide; }

bool Move::getIsCastlingQueenSide() const { return isCastlingQueenSide; }

void Move::setIsPromotion(bool promotion) { isPromotion = promotion; }

void Move::setIsCastlingKingSide(bool castling) { isCastlingKingSide = castling; }

void Move::setIsCastlingQueenSide(bool castling) { isCastlingQueenSide = castling; }

void Move::setPieceMoved(Piece* piece) { pieceMoved = piece; }

void Move::setPieceCaptured(Piece* piece) { pieceCaptured = piece; }

void Move::setFrom(const Position& from) { this->from = from; }

void Move::setTo(const Position& to) { this->to = to; }