cmake_minimum_required(VERSION 3.23)
project(Connect3)

set(CMAKE_CXX_STANDARD 14)

add_executable(Connect3 main.cpp Chess.cpp Chess.h Game.cpp Game.h)
