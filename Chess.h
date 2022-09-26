//
// Created by FRED SHI on 8/7/2022.
//

#ifndef CONNECT3_CHESS_H
#define CONNECT3_CHESS_H


enum class Role {
    None,
    Knight,
    King,
    Queen,
    Pawn,
    Rook,
    Bishop
};

struct Position {
    int row;
    int column;

    Position(int r, int c);
    Position();

    bool operator==(const Position &pos) const;

};

class Character{
public:
    Role role_;
    Position position_{0,0};
    bool is_captured_;
    bool operator == (const Character& c) const;

    Character(Role role, Position position);
};

#endif //CONNECT3_CHESS_H
