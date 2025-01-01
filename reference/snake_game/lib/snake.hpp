#pragma once
#include <bits/stdc++.h>
#include <windows.h>
#include "map.hpp"
#include "snake.hpp"
#include "food.hpp"
#include "key.hpp"
#include "settings.hpp"
#include "def.hpp"
using namespace std;


class Map;
class Food;

class Snake {
public:
    Snake(Coord start_position, Settings *_settings);
    void move(Map &map, Food &food);
    vector<Coord> get_body();
    bool is_game_over();
    bool is_head(Coord a);
    Coord get_begin();
    Coord get_end();
    int get_score();
    Settings* settings;
private:
    KeyManager key;
    deque<Coord> body;
    char direction;
    int score;
    bool game_over;
    char reverse_dir(char dir);
};


