#include "map.hpp"
#include "snake.hpp"
#include "food.hpp"
#include "key.hpp"
#include "settings.hpp"
#include "def.hpp"

using namespace std; 
    
Food::Food(Settings *_settings)
    : map_width(_settings->map_size.X), map_height(_settings->map_size.Y), settings(_settings)
{
    srand(time(0));
}

void Food::spawn_initial_food(Map &map, Snake &snake, int n)
{
    for (int i = 0; i < n; i++)
        spawn_random_food(map, snake);
    return;
}

void Food::spawn_random_food(Map &map, Snake &snake)
{
    Coord new_food;
    do{
        new_food.X = rand() % (map_width - 2) + 1;
        new_food.Y = rand() % (map_height - 2) + 1;
    } while (is_occupied(new_food, snake, map));
    food_items.pb({new_food,settings->food_lifespan*1000,true});
    return;
}

bool Food::check_food_collision(Coord &coord)
{
    for (vector<Food_item>::iterator it = food_items.begin(); it != food_items.end(); it++)
    {
        if (it->position.X == coord.X && it->position.Y == coord.Y)
        {
            food_items.erase(it);
            return true;
        }
    }
    return false;
}

vector<Food_item> Food::get_food_coordinates()
{
    return food_items;
}

bool Food::is_occupied(Coord &coord, Snake &snake, Map &map)
{
    for (Coord &segment : snake.get_body())
        if (segment.X == coord.X && segment.Y == coord.Y)
            return true;
    if (map.collide_wall(coord))
        return true;
    return false;
}

void Food::update_food_lifespans(Map &map, Snake &snake){
    for (vector<Food_item>::iterator it = food_items.begin(); it != food_items.end();) {
        if (it->lifespan > 3000) {
            it->lifespan -= settings->snake_cur_speed;
            it++;
#ifdef DEBUG
            cout_val(settings->snake_cur_speed);
            cout_val(settings->snake_init_speed);
#endif
        }
        else if(it->lifespan > 0){
            it->lifespan -= settings->snake_cur_speed;
            it++;
            it->display = (!(it->display));
        }
        else{
            it = food_items.erase(it);
            spawn_random_food(map, snake);
        }
    }
}
