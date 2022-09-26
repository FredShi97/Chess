//
// Created by FRED SHI on 8/7/2022.
//

#ifndef CONNECT3_GAME_H
#define CONNECT3_GAME_H


#include <vector>
#include <string>
#include "Chess.h"


struct Round{
    std::string black_move;
    std::string white_move;
};

enum class Player{
    None,
    Black,
    White
};


class Game {

public:

    bool black_check_;
    bool white_check_;
    bool game_over_;

    std::tuple<Role, Player> FindCharacterWithPos(Position pos);
    void UpdateMove(Player player, Position current_pos, Position next_pos);
    bool IsValidMove (Player player, Position current_pos, Position next_pos);
    Game();

private:

    std::vector<Round> history_;
    std::vector<Character> black_char_list_;
    std::vector<Character> white_char_list_;
    std::vector<Character> check_king_list_;
    std::vector<Position> king_possible_pos_;
    Character* current_char_;
    Position white_king_pos_;
    Position black_king_pos_;

    bool IsPosAvailable(Player player, Position p);
    bool IsValidMoveCheck(Character* character, Player player, Position next_pos);
    void CheckAndCaptureEnemy(Character* character, Player player, Position next_pos);
    void IsCheck(Player player);
    void IsCheckMate(Player player);
    void FindKingPossibleMove(Player player, Position current_pos);
    void InitializeBoard();
    bool SearchVertical(const Character& character, Position start_pos, Position end_pos);
    bool SearchHorizontal(const Character& character, Position start_pos, Position end_pos);
    bool SearchDiagonal(const Character& character, Position start_pos, Position end_pos);
    std::vector<Character>::iterator FindCharacterWithPos(Position pos, Player search_range);

    bool IsBlockedInStraightLine(Position &p1, Position &p2);
    bool IsBlockedInDiagonalLine(Position &p1, Position &p2);
};


#endif //CONNECT3_GAME_H
