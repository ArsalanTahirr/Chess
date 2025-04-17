#pragma once

enum class Color {
    White,
    Black
};

enum class PieceType {
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn
};


enum class GameState {
    NOT_STARTED,
    IN_PROGRESS,
    CHECK,
    CHECKMATE,
    STALEMATE,
    DRAW,
    RESIGNED
};