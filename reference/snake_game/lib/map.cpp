#include "map.hpp"
#include "snake.hpp"
#include "food.hpp"
#include "key.hpp"
#include "settings.hpp"
#include "def.hpp"
using namespace std;

Map::Map(Coord &snake_start, Settings *_settings) 
    :map_width(_settings->map_size.X + 2), map_height(_settings->map_size.Y + 2), wall_dense(_settings->wall_dense), settings(_settings)
{
    map.resize(map_height, string(map_width, ' '));
    for (int i = 0; i < map_height; i++)
    {
        Coord a, b;
        a.X = i;
        a.Y = 0;
        b.X = i;
        b.Y = map_width - 1;
        wall.pb(a);
        wall.pb(b);
    }
    for (int i = 1; i < map_width - 1; i++)
    {
        Coord a, b;
        a.X = 0;
        a.Y = i;
        b.X = map_height - 1;
        b.Y = i;
        wall.pb(a);
        wall.pb(b);
    }
    srand(time(0));
    double tmp = map_width * map_height * wall_dense / 100;
    wall_cnt = round(tmp);
    for (int i = 0; i < wall_cnt; i++)
    {
        Coord a;
        a.X = (rand() % (map_height-2)) +1;
        a.Y = (rand() % (map_width-2)) +1;
        if (abs(a.X-snake_start.X)>3 || abs(a.Y-snake_start.Y)>3) wall.pb(a);
        else i--;
    }
    for (Coord &i : wall)
        map[i.X][i.Y] = '#';
    display_ver_update();
    
}

Map::~Map()
{
}

void Map::initialize(Snake &snake, Food &food)
{
    clear_all;
    return;
}

void Map::render(Snake &snake, Food &food)
{
    vector<Coord> snake_body = snake.get_body();
    vector<Food_item> food_coords = food.get_food_coordinates();
    map.resize(0);
    map.resize(map_height, string(map_width, ' '));
    for (Coord &i : wall)
        map[i.X][i.Y] = '#';
    for (Food_item &i : food_coords)
        if(i.display) map[i.position.Y][i.position.X] = '*';
    for (size_t i = 0; i < snake_body.size(); i++)
        map[snake_body[i].Y][snake_body[i].X] = (i==0) ? 'O' : '.';
    display_ver_update();
#ifndef DEBUG
    clear_all;
#endif
    cout << "Score: " << snake.get_score() << endl;
    for (string &row : map_display)//
        cout << row << endl;
#ifdef DEBUG
    cout_val(settings->cur_negative_power_up_time);
#endif
    
    return;
}

bool Map::out_of_bounds(Coord a)
{
    return a.X <= 0 || a.X >= map_width - 1 || a.Y <= 0 || a.Y >= map_height - 1;
}

bool Map::collide_wall(Coord a)
{
    for (Coord &i : wall) if(i.Y == a.X && i.X == a.Y) return true;
    return false;
}

void Map::display_ver_update(){
    map_display.resize(0);
    map_display.resize(map_height);
    for (int i=0; i<map_height; i++){
        for(int j=0 ; j<map_width; j++){
            if(map[i][j]=='#'){
                function<bool(int,int)> check_up = [&](int x,int y){
                    if(0<=x && x<map_height && 0<=y && x<map_width)
                        if(map[x][y]=='#')
                            return 1;
                    return 0;
                };
                int i_check=check_up(i-1,j)*8+//w
                            check_up(i,j+1)*4+//d
                            check_up(i+1,j)*2+//s
                            check_up(i,j-1);  //a
                string type[16]={"X","─","│","┐","─","─","┌","┬","│","┘","│","┤","└","┴","├","┼"};//─│┌┐└┘├┤┬┴┼
                map_display[i]+=type[i_check];
            }
            else{
                map_display[i]+=map[i][j];
            }
            
        }
    }
}
