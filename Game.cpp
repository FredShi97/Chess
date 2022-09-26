//
// Created by FRED SHI on 8/7/2022.
//

#include <algorithm>
#include <tuple>
#include "Game.h"


Game::Game() {
    black_char_list_.clear();
    white_char_list_.clear();
    king_possible_pos_.clear();
    check_king_list_.clear();
    history_.clear();

    black_check_ = false;
    white_check_ = false;
    game_over_ = false;

    current_char_ = nullptr;

    InitializeBoard();

}


std::tuple<Role, Player> Game::FindCharacterWithPos(Position pos) {


    std::vector<Character>::iterator it = FindCharacterWithPos(pos, Player::White);
    if (it != white_char_list_.end())
        return std::make_tuple(it->role_, Player::White);
    else{
        it = FindCharacterWithPos(pos, Player::Black);
        if (it != black_char_list_.end())
            return std::make_tuple(it->role_, Player::Black);
    }


    return std::make_tuple(Role::None, Player::None);
}

std::vector<Character>::iterator Game::FindCharacterWithPos(Position pos, Player search_range) {

    std::vector<Character>::iterator it;

    if (search_range == Player::Black){
        for (it = black_char_list_.begin(); it < black_char_list_.end(); ++it){
            if(it->position_ == pos) return it;
        }
    }
    else{
        for (it = white_char_list_.begin(); it < white_char_list_.end(); ++it){
            if (it->position_ == pos) return it;
        }
    }

    return it;

}

void Game::IsCheck(Player player) {

    check_king_list_.clear();

    //clear the check king list, calculate for new ones.

    if (player == Player::Black){
        for (Character &item : black_char_list_){
            if (IsValidMoveCheck(&item, player, white_king_pos_)) check_king_list_.push_back(item);
        }
    }
    else {
        for (Character &item : white_char_list_){
            if (IsValidMoveCheck(&item, player, black_king_pos_)) check_king_list_.push_back(item);
        }
    }
}

bool Game::IsPosAvailable(Player player, Position p) {
    if (p.row >= 0 && p.row <= 7  && p.column < 0 && p.column > 7){

        std::vector<Character>::iterator it = FindCharacterWithPos(p,player);

        //can not find a character at position, so position is available
        if ((it == black_char_list_.end() && player == Player::Black) ||
            (it == white_char_list_.end() && player == Player::White))

            return true;
        else
            return false;
    }

    return false;

}


void Game::FindKingPossibleMove(Player player, Position current_pos) {

    king_possible_pos_.clear();
    Position p = Position();

    for (int i = -1; i < 1; i++){
        for (int j = -1; j < 1; j++){
            //do not want itself, loop through 8 positions
            if (i == 0 && j == 0) continue;

            p = Position(current_pos.row + i, current_pos.column + j);
            if (IsPosAvailable(player, p)) king_possible_pos_.push_back(p);
        }
    }

}

void Game::IsCheckMate(Player player) {

    bool can_capture = false;
    bool can_block = false;
    Position king_pos = (player == Player::Black ? black_king_pos_ : white_king_pos_);
    int safe_pos = 8;

    if (check_king_list_.size() == 1){
        //only 1 checking the king, can choose to escape, block, capture
        Position possible_move = check_king_list_[0].position_;
        if (player == Player::Black) {
            //if the current player is black, want to search if any white can capture this black
            for (Character &item: white_char_list_) {
                if (IsValidMoveCheck(&item, Player::White, possible_move)) {
                    can_capture = true;
                    break;
                }
            }
        }
        else{
            //if the current player is black, want to search if any white can capture this black
            for (Character &item: black_char_list_) {
                if (IsValidMoveCheck(&item, Player::Black, possible_move)) {
                    can_capture = true;
                    break;
                }
            }
        }

        if (!can_capture){
            //check for can block
            ;
        }

        if ((!can_capture) && (!can_block)){

            FindKingPossibleMove(player, king_pos);
            safe_pos = king_possible_pos_.size();

            for (Position &pos : king_possible_pos_){
                if (player == Player::Black) {
                    //if the current player is black, want to search if any white can capture this black
                    for (Character &item: white_char_list_) {
                        if (IsValidMoveCheck(&item, Player::White, pos)) {
                            --safe_pos;
                            break;
                        }
                    }
                }
                else{
                    //if the current player is black, want to search if any white can capture this black
                    for (Character &item: black_char_list_) {
                        if (IsValidMoveCheck(&item, Player::Black, pos)) {
                            --safe_pos;
                            break;
                        }
                    }
                }
            }

        }

    }
    else if (check_king_list_.size() >= 2){
        //2 or more checking the king, can only choose to escape
        FindKingPossibleMove(player, king_pos);
        safe_pos = king_possible_pos_.size();

        for (Position &pos : king_possible_pos_){
            if (player == Player::Black) {
                //if the current player is black, want to search if any white can capture this black
                for (Character &item: white_char_list_) {
                    if (IsValidMoveCheck(&item, Player::White, pos)) {
                        --safe_pos;
                        break;
                    }
                }
            }
            else{
                //if the current player is black, want to search if any white can capture this black
                for (Character &item: black_char_list_) {
                    if (IsValidMoveCheck(&item, Player::Black, pos)) {
                        --safe_pos;
                        break;
                    }
                }
            }
        }
    }

    game_over_ = (!can_capture) && (!can_block) && (safe_pos == 0);
}


bool Game::SearchVertical(const Character& character,Position start_pos, Position end_pos) {

    //is vertical move
    if (start_pos.row < end_pos.row &&
        character.position_.row > start_pos.row &&
        character.position_.row < end_pos.row)
            //move forward
            return true;
    else if (start_pos.row > end_pos.row &&
        character.position_.row > end_pos.row &&
        character.position_.row < start_pos.row)
            //move backward
            return true;

    return false;
}

bool Game::SearchHorizontal(const Character& character, Position start_pos, Position end_pos) {

    if (start_pos.column < end_pos.column &&
        character.position_.column > start_pos.column &&
        character.position_.column < end_pos.column)
            return true;
    else if (start_pos.column > end_pos.column &&
        character.position_.column > end_pos.column &&
        character.position_.column < start_pos.column)
            return true;

    return false;
}

bool Game::SearchDiagonal(const Character& character, Position start_pos, Position end_pos) {

    //is diagonal
    if (abs(character.position_.row - start_pos.row) ==
    abs(character.position_.column - start_pos.column)) {

        //within range
        if (SearchVertical(character,start_pos, end_pos) &&
                SearchHorizontal(character, start_pos, end_pos))
            return  true;
    }


    return false;
}


bool Game::IsValidMoveCheck(Character* character, Player player, Position next_pos) {

    if (next_pos.row < 0 || next_pos.row > 7 ||
            next_pos.column < 0 || next_pos.column > 7)
        return false;

    Position p1 = character->position_;
    Position p2 = next_pos;
    bool is_valid = false;



    //if end_pos has same side character? the move is invalid, early exit
    std::vector<Character>::iterator it = FindCharacterWithPos(next_pos, player);
    if ((it != black_char_list_.end() && player == Player::Black) ||
    (it != white_char_list_.end() && player == Player::White))
        return false;

    switch(character->role_){
        case Role::Knight:

            //knight moves vertical or horizontal 1 and then diagonal 1.
            //total move, either vertical 1, horizontal 2
            if ((abs(p1.row - p2.row) == 1 && abs(p1.column - p2.column) == 2) ||
            //or horizontal 1 vertical 2
            (abs(p1.column - p2.column) == 1 && abs(p1.row - p2.row) == 2)) {
                //knight can jump over character, no need to validate if a character is in its way
                is_valid = true;
            }

            break;
        case Role::Bishop:

            //is valid move
            if (abs(p1.row - p2.row) == abs(p1.column - p2.column)) {

                is_valid = !IsBlockedInDiagonalLine(p1, p2);
            }

            break;

        case Role::Rook:

            if ((p1.row == p2.row && p1.column != p2.column) ||
                    (p1.column == p2.column && p1.row != p2.row)) {
                //is blocked in straight line checks both horizontal and vertical
                is_valid = !IsBlockedInStraightLine(p1, p2);
            }

            break;

        case Role::Queen:
            //diag check
            if (abs(p1.row - p2.row) == abs(p1.column - p2.column)) {

                is_valid = !IsBlockedInDiagonalLine(p1, p2);
            }
            //vertical or horizontal check
            else if  ((p1.row == p2.row && p1.column != p2.column) ||
                (p1.column == p2.column && p1.row != p2.row)) {
                //is blocked in straight line checks both horizontal and vertical
                is_valid = !IsBlockedInStraightLine(p1, p2);

            }

            break;

        case Role::King:

            //king can only move 1 block
            if ((p1.row == p2.row && abs(p1.column - p2.column) == 1)||
                (p1.column == p2.column && abs(p1.row - p2.row) == 1)) {
                //does need to check if its blocked
                is_valid = true;

                //updating global king position, so IsCheck can find all char that can check the king
                if (player == Player::Black)
                    black_king_pos_ = next_pos;
                else
                    white_king_pos_ = next_pos;
            }

            break;

        case Role::Pawn:

            if ((p1.row == p2.row && abs(p1.column - p2.column) == 1)||
                (p1.column == p2.column && abs(p1.row - p2.row) == 1)) {
                //does need to check if its blocked
                is_valid = true;
            }

            break;
    }

    return is_valid;
}

bool Game::IsBlockedInDiagonalLine(Position &p1, Position &p2) {

    std::vector<Character>::iterator it;

    it = std::find_if(black_char_list_.begin(), black_char_list_.end(),
                      [p1, p2, this](const Character& c) {return SearchDiagonal(c, p1, p2);});
    if (it != black_char_list_.end())
        return true;

    it = std::find_if(white_char_list_.begin(), white_char_list_.end(),
                      [p1, p2, this](const Character& c) {return SearchDiagonal(c, p1, p2);});

    if (it != white_char_list_.end())
        return true;

    return false;
}

bool Game::IsBlockedInStraightLine(Position &p1, Position &p2) {

    std::vector<Character>::iterator it;

    it = std::find_if(black_char_list_.begin(), black_char_list_.end(),
                      [p1, p2, this](const Character &c) { return SearchHorizontal(c, p1, p2); });
    if (it != black_char_list_.end())
        return true;

    it = std::find_if(white_char_list_.begin(), white_char_list_.end(),
                      [p1, p2, this](const Character &c) { return SearchHorizontal(c, p1, p2); });

    if (it != white_char_list_.end())
        return true;

    it = std::find_if(white_char_list_.begin(), white_char_list_.end(),
                      [p1, p2, this](const Character &c) { return SearchVertical(c, p1, p2); });

    if (it != white_char_list_.end())
        return true;

    it = std::find_if(white_char_list_.begin(), white_char_list_.end(),
                      [p1, p2, this](const Character &c) { return SearchVertical(c, p1, p2); });

    if (it != white_char_list_.end())
        return true;

    return false;
}

//check if enemy can be captured, if true, pop it off queue
void Game::CheckAndCaptureEnemy(Character* character, Player player, Position next_pos) {

    Player opponent = (player == Player::Black) ? Player::White : Player::Black;
    std::vector<Character>::iterator it = FindCharacterWithPos(character->position_, opponent);
    if (it != black_char_list_.end() && opponent == Player::Black){
        black_char_list_.erase(it);
    }
    else if (it != white_char_list_.end() && opponent == Player::White){
        white_char_list_.erase(it);
    }

}

bool Game::IsValidMove(Player player, Position current_pos, Position next_pos) {


    std::vector<Character>::iterator it = FindCharacterWithPos(current_pos, player);
    current_char_ = nullptr;
    if ((it != black_char_list_.end() && player == Player::Black)
        || (it != white_char_list_.end() && player == Player::White))
        //dereference iterator to get the object, and then find the address
        current_char_ = &(*it);

    return IsValidMoveCheck(current_char_, player, next_pos);
}

void Game::UpdateMove(Player player, Position current_pos, Position next_pos) {

    //check for nullptr
    if (current_char_ != nullptr){

        CheckAndCaptureEnemy(current_char_, player, next_pos);

        current_char_->position_ = next_pos;

        IsCheck(player);
        IsCheckMate(player);

        //also detect if its checkmate

        //finish using ptr
        current_char_ = nullptr;
    }

}

void Game::InitializeBoard() {
    for (int column = 0; column < 8; column++){
        black_char_list_.push_back(Character(Role::Pawn, Position(1, column)));
        white_char_list_.push_back(Character(Role::Pawn, Position(6, column)));
    }

    black_char_list_.push_back(Character(Role::Rook, Position(0, 0)));
    black_char_list_.push_back(Character(Role::Knight, Position(0, 1)));
    black_char_list_.push_back(Character(Role::Bishop, Position(0, 2)));
    black_char_list_.push_back(Character(Role::Queen, Position(0, 3)));
    black_char_list_.push_back(Character(Role::King, Position(0, 4)));
    black_char_list_.push_back(Character(Role::Bishop, Position(0, 5)));
    black_char_list_.push_back(Character(Role::Knight, Position(0, 6)));
    black_char_list_.push_back(Character(Role::Rook, Position(0, 7)));


    white_char_list_.push_back(Character(Role::Rook, Position(7, 0)));
    white_char_list_.push_back(Character(Role::Knight, Position(7, 1)));
    white_char_list_.push_back(Character(Role::Bishop, Position(7, 2)));
    white_char_list_.push_back(Character(Role::Queen, Position(7, 3)));
    white_char_list_.push_back(Character(Role::King, Position(7, 4)));
    white_char_list_.push_back(Character(Role::Bishop, Position(7, 5)));
    white_char_list_.push_back(Character(Role::Knight, Position(7, 6)));
    white_char_list_.push_back(Character(Role::Rook, Position(7, 7)));


    //initializing king position along with the board.
    white_king_pos_ = Position(7, 4);
    black_king_pos_ = Position(0, 4);

}