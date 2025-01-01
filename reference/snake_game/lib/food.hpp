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

// Forward declarations
class Map;
class Snake;

struct Food_item {
    Coord position;
    int lifespan;
    bool display;
};

class Food {
public:
    Food(Settings *_settings);
    void spawn_initial_food(Map &map, Snake &snake, int n);
    void spawn_random_food(Map &map, Snake &snake);
    bool check_food_collision(Coord &coord);
    void update_food_lifespans(Map &map, Snake &snake);
    void render_food(Map &map);
    vector<Food_item> get_food_coordinates();
    Settings* settings;

private:
    bool is_occupied(Coord &coord, Snake &snake, Map &map);
    vector<Food_item> food_items;
    int map_width, map_height;
};


