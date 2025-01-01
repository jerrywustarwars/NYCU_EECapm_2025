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

class Map {
public:
    Map(Coord &sanke_start, Settings *_settings);
    ~Map();
    void initialize(Snake &snake, Food &food);
    void render(Snake &snake, Food &food);
    bool out_of_bounds(Coord a);
    bool collide_wall(Coord a);
    void display_ver_update();
    int map_width, map_height;
    Settings* settings;
private:
    
    vector<string> map;
    vector<string> map_display;
    vector<Coord> wall;
    int wall_dense;
    int wall_cnt;
};
