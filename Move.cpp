#include <iostream>
#include "Move.h"
#include "Piece.h"

Move::Move(const Position& from, const Position& to, Piece* pieceMoved, Piece* pieceCaptured):
    from(from), to(to), pieceMoved(pieceMoved), pieceCaptured(pieceCaptured),
    isCheck(false), isCheckmate(false), isPromotion(false), isCastling(false), isEnPassant(false) {}

Move::~Move() {}

Position Move::getFrom() const { return from; }

Position Move::getTo() const { return to; }

Piece* Move::getPieceMoved() const { return pieceMoved; }

Piece* Move::getPieceCaptured() const { return pieceCaptured; }

bool Move::getIsCheck() const { return isCheck; }

bool Move::getIsCheckmate() const { return isCheckmate; }

bool Move::getIsPromotion() const { return isPromotion; }

bool Move::getIsCastling() const { return isCastling; }

bool Move::getIsEnPassant() const { return isEnPassant; }

void Move::setIsCheck(bool check) { isCheck = check; }

void Move::setIsCheckmate(bool checkmate) { isCheckmate = checkmate; }

void Move::setIsPromotion(bool promotion) { isPromotion = promotion; }

void Move::setIsCastling(bool castling) { isCastling = castling; }

void Move::setIsEnPassant(bool enPassant) { isEnPassant = enPassant; }