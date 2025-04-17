#pragma once

#include "Position.h"

class Piece;

class Move {
private:
    Position from;
    Position to;
    Piece* pieceMoved;
    Piece* pieceCaptured;
    bool isCheck;
    bool isCheckmate;
    bool isPromotion;
    bool isCastling;
    bool isEnPassant;
public:
    Move(const Position& from, const Position& to, Piece* pieceMoved, Piece* pieceCaptured);
    ~Move();
    Position getFrom() const;
    Position getTo() const;
    Piece* getPieceMoved() const;
    Piece* getPieceCaptured() const;
    bool getIsCheck() const;
    bool getIsCheckmate() const;
    bool getIsPromotion() const;
    bool getIsCastling() const;
    bool getIsEnPassant() const;
    void setIsCheck(bool check);
    void setIsCheckmate(bool checkmate);
    void setIsPromotion(bool promotion);
    void setIsCastling(bool castling);
    void setIsEnPassant(bool enPassant);
};