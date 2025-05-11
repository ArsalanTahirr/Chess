#pragma once

#include "Position.h"
#include "Enums.h"

class Piece;

class Move {
private:
    Position from;
    Position to;
    Piece* pieceMoved;
    Piece* pieceCaptured;
    PieceType pieceType;
    bool isPromotion;
    bool isCastlingKingSide;
    bool isCastlingQueenSide;
public:
    Move();
    ~Move();
    Position getFrom() const;
    Position getTo() const;
    Piece* getPieceMoved() const;
    Piece* getPieceCaptured() const;
    bool getIsPromotion() const;
    bool getIsCastlingKingSide() const;
    bool getIsCastlingQueenSide() const;
    PieceType getPieceType() const;
    void setIsPromotion(bool promotion);
    void setIsCastlingKingSide(bool castling);
    void setIsCastlingQueenSide(bool castling);
    void setPieceType(PieceType type);
    void setPieceMoved(Piece* piece);
    void setPieceCaptured(Piece* piece);
    void setFrom(const Position& from);
    void setTo(const Position& to);
};