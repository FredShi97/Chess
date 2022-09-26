//
// Created by FRED SHI on 8/7/2022.
//

#include "Chess.h"

Position::Position(int r, int c) {
    row = r;
    column = c;
}

Position::Position(): Position{0,0}{}


bool Position::operator==(const Position &pos) const {
    if (this->row == pos.row && this->column == pos.column)
        return true;
    return false;
}

bool Character::operator == (const Character& c) const { return position_.row == c.position_.row && position_.column == c.position_.column;}

Character::Character(Role role, Position position) {
    role_ = role;
    position_ = position;
}