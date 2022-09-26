#include <iostream>
#include <tuple>
#include "Game.h"

char const white = 0xDB;
char const black  = 0xFF;



std::string GetRoleAtLocation(Role role, Player player){
    std::string name = "";
    if (player == Player::Black)
        name.push_back('B');
    else
        name.push_back('W');


    switch(role){
        case Role::Knight:
            name.push_back('N');
            break;
        case Role::King:
            name.push_back('K');
            break;
        case Role::Queen:
            name.push_back('Q');
            break;
        case Role::Pawn:
            name.push_back('P');
            break;
        case Role::Rook:
            name.push_back('R');
            break;
        case Role::Bishop:
            name.push_back('B');
            break;
    }

    return name;
}

int GetColumn(char col){

    char upper = toupper(col);

    switch (upper){
        case 'A':
            return 0;
            break;
        case 'B':
            return 1;
            break;
        case 'C':
            return 2;
            break;
        case 'D':
            return 3;
            break;
        case 'E':
            return 4;
            break;
        case 'F':
            return 5;
            break;
        case 'G':
            return 6;
            break;
        case 'H':
            return 7;
            break;
    }
}

std::tuple<Position, Position> ReadInput(std::string in){
    Position current = Position((in[1] - '0'), GetColumn(in[0]));
    Position next = Position((in[4] - '0'), GetColumn(in[3]));

    return {current, next};
}

void PrintLineOnBoard(Game &game, int row){

    char first_color = white;
    char second_color = black;

    if (row % 2 == 0){
        first_color = black;
        second_color = white;
    }

    for(int index = 0; index < 8; index++) {

        if (index % 2 == 0) {
            std::cout << first_color << first_color << first_color << first_color << first_color << first_color << first_color;
        } else {
            std::cout << second_color << second_color << second_color << second_color << second_color << second_color
                      << second_color;
        }
    }
    std::cout <<  "\n" ;


    for(int index = 0; index < 8; index++){

        auto [role, player] = game.FindCharacterWithPos(Position(row, index));

        if (role == Role::None && player == Player::None){
            if (index % 2 == 0) {
                std::cout << first_color << first_color << first_color << first_color << first_color << first_color << first_color;
            } else {
                std::cout << second_color << second_color << second_color << second_color << second_color << second_color <<second_color;
            }
        }
        else {
            std::string role_str = GetRoleAtLocation(role, player);

            if (index % 2 == 0) {
                std::cout << first_color << first_color << first_color << role_str  << first_color << first_color;
            } else {
                std::cout << second_color << second_color << second_color << role_str  << second_color <<second_color;
            }
        }
    }

    std::cout << " " << row;
    std::cout <<  "\n" ;


    for(int index = 0; index < 8; index++) {

        if (index % 2 == 0) {
            std::cout << first_color << first_color << first_color << first_color << first_color << first_color << first_color;
        } else {
            std::cout << second_color << second_color << second_color << second_color << second_color <<second_color
                      << second_color;
        }
    }

    std::cout <<  "\n" ;


}

int main() {

    Game game = Game();


    std::cout << "  A       B      C      D      E      F      G      H" << std::endl;

    for (int row = 0; row < 8; row++){
        PrintLineOnBoard(game, row);
    }

    bool is_black_turn = false;
    bool is_valid = false;
    std::string move = "";
    Position p1 = Position(0,0);
    Position p2 = Position(0, 0);
    Player player = Player::None;

    while (!game.game_over_){

        std::cout << "make next move    " << ((is_black_turn) ? "black" : "white") << "  turn" << std::endl;

        player = (is_black_turn) ? Player::Black : Player::White;


        std::cin >> move;
        std::tie(p1, p2) = ReadInput(move);

        is_valid = game.IsValidMove(player, p1, p2);

        while (!is_valid){
            std::cout << "move invalid" << std::endl;
            std::cout << "make next move    " << ((is_black_turn) ? "black" : "white") << "  turn" << std::endl;
            std::cin >> move;
            std::tie(p1, p2) = ReadInput(move);

            is_valid = game.IsValidMove(player, p1, p2);

        }

        game.UpdateMove(player, p1, p2);
        //switch turnB2
        is_black_turn = !is_black_turn;

        //reprint board
        std::cout << "  A       B      C      D      E      F      G      H" << std::endl;
        for (int row = 0; row < 8; row++){
            PrintLineOnBoard(game, row);
        }

    }


    return 0;
}


