#pragma once

#include <bits/stdc++.h>
#include <windows.h>
#include "map.hpp"
#include "game.hpp"
#include "snake.hpp"
#include "food.hpp"
#include "key.hpp"
#include "settings.hpp"
#include "def.hpp"
using namespace std;

    
    
class Game{
public:
    Game(Settings *_settings);
    void starting();
    Settings* settings;
private:
    Coord map_size;
    int init_food;
    int cur_food;
};