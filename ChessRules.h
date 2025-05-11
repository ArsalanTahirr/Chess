#pragma once

#include "Enums.h"

class Move;
class Board;
class Piece;
class StateString;
class Player;

class ChessRules {
public:
    ChessRules();
    ~ChessRules();
    static bool isCheck(Color color, Board* board);
    static bool isCheckmate(Color color, Board* board);
    static bool isStalemate(Color color, Board* board);
    static bool canCastleKingSide(Color color, Board* board);
    static bool canCastleQueenSide(Color color, Board* board);
    static bool isCastleLegal(Color color, Board*board, int moveDirection);
    static std::pair<bool, std::string> isDraw(std::vector<StateString>& stateHistory, Board* board);
    static bool hasInsufficientMaterial(Board* board);
    static bool hasThreefoldRepetition(std::vector<StateString>& stateHistory);
    static void handleCastlingKingSide(Player& player, Board* board);
    static void handleCastlingQueenSide(Player& player, Board* board);
    static void handlePromotion(Player& player, PieceType& type, Move* move, Board* board);
};