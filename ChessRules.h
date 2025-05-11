#pragma once

#include "Enums.h"

class Move;
class Board;
class Piece;
class MoveHistory;

class ChessRules {
public:
    ChessRules();
    ~ChessRules();
    //static bool isValidMove(Move* move, Board* board);
    static bool isCheck(Color& color, Board* board, MoveHistory* moveHistory);
    static bool isCheckmate(Color& color, Board* board, MoveHistory* moveHistory);
    static bool isStalemate(Color& color, Board* board, MoveHistory* moveHistory);
    static bool isCastleLegal(Color color, Board*board, int moveDirection);
    //static bool isDraw(MoveHistory& moveHistory, Board* board);
    static bool hasInsufficientMaterial(Board* board);
    static bool hasThreefoldRepetition(Board* board);
    // static void handleCastling(Move* move, Board* board);
    // static void handleEnPassant(Move* move, Board* board);
    // static void handlePromotion(Move* move, PieceType& type, Board* board);
};