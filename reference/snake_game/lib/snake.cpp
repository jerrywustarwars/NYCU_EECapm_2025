#include "map.hpp"
#include "snake.hpp"
#include "food.hpp"
#include "key.hpp"
#include "settings.hpp"
#include "def.hpp"
using namespace std;
    
Snake::Snake(Coord start_position, Settings *_settings) 
    : direction('d'), score(0), game_over(false), settings(_settings)
{
    body.push_back(start_position);
}

void Snake::move(Map &map, Food &food)
{
    if(direction != key.last_dir && settings->negative_power_up && settings->cur_negative_power_up_time > 0){
        int tmp = settings->cur_negative_power_up_time;
        settings->change_cur_negative_power_up_time(tmp-1);
        direction = reverse_dir(key.last_dir);
    }
    else if(direction != reverse_dir(key.last_dir) && key.new_dir_input()){
        direction = key.last_dir;
    }
    Coord new_head = body.front();
    switch (direction)
    {
    case 'w':
        --new_head.Y;
        break;
    case 's':
        ++new_head.Y;
        break;
    case 'a':
        --new_head.X;
        break;
    case 'd':
        ++new_head.X;
        break;
    }
    if(settings->boundary_wrap_around)
    {
        if (new_head.X < 1) {
            new_head.X = map.map_width - 2;
        } else if (new_head.X >= map.map_width - 1) {
            new_head.X = 1;
        }
        if (new_head.Y < 1) {
            new_head.Y = map.map_height - 2;
        } else if (new_head.Y >= map.map_height - 1) {
            new_head.Y = 1;
        }
    }

    
    if (map.collide_wall(new_head))
    {
        game_over = true;
        return;
    }

    for (int i = 1; i < body.size(); i++)
    {
        if (body[i].X == new_head.X && body[i].Y == new_head.Y)
        {
            while(body.back().X != new_head.X || body.back().Y != new_head.Y)
            {
                body.pop_back();
                score--;
            }
            if(settings->negative_power_up) settings->change_cur_negative_power_up_time(settings->init_negative_power_up_time);
        }
    }

    body.push_front(new_head);

    if (food.check_food_collision(new_head))
    {
        score++;
        food.spawn_random_food(map, *this);
        reverse(all(body));

        Coord first = body.front();
        Coord second = body[1];
        if (first.X == second.X) {
            if (first.Y < second.Y) {
                direction = 'w';
            } else {
                direction = 's';
            }
        } else if (first.Y == second.Y) {
            if (first.X < second.X) {
                direction = 'a';
            } else {
                direction = 'd';
            }
        }
        key.used_dir();
    }
    else
    {
        body.pop_back();
    }
#ifdef DEBUG
    cout_val(direction);
    cout_val(key.last_dir);
    cout_val(key.last_key);
#endif
    
    return;
}

vector<Coord> Snake::get_body()
{
    return vector<Coord>(all(body));
}

bool Snake::is_head(Coord a){
    return (body.front().X == a.X && body.front().Y == a.Y);
}

Coord Snake::get_begin()
{
    return body.front();
}
Coord Snake::get_end()
{
    return body.back();
}

bool Snake::is_game_over()
{
    return game_over;
}

int Snake::get_score()
{
    return score;
}

char Snake::reverse_dir(char dir)
{
    switch(dir){
        case 'w': return 's'; 
        case 'a': return 'd';
        case 's': return 'w';
        case 'd': return 'a';
        default : return ' ';
    }
}