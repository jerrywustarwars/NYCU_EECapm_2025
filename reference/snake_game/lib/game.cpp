#include "map.hpp"
#include "game.hpp"
#include "snake.hpp"
#include "food.hpp"
#include "key.hpp"
#include "settings.hpp"
#include "def.hpp"
using namespace std;
    

Game::Game(Settings *_settings)
    :map_size(_settings->map_size),init_food(_settings->food_cnt),cur_food(_settings->food_cnt), settings(_settings){
    
    starting();
    
    Coord start_position = {map_size.X/2, map_size.Y/2};
    if(settings->rand_start) start_position = {rand()%(map_size.X-4)+2, rand()%(map_size.Y-4)+2};
    Snake game_snake(start_position, settings);
    Map game_map(start_position,settings);
    Food game_food(settings);
    game_map.initialize(game_snake, game_food);
    game_food.spawn_initial_food(game_map, game_snake, init_food);
    clear_all;
    for(int i=3;i>0;i--){
        game_map.render(game_snake, game_food);
        cout<<"starting in "<<i<<" . . .";
        Sleep(1000);
        clear_all;
    }
    game_map.render(game_snake, game_food);
    cout<<"start!!!";
    // Sleep(1000);
    // clear_all;
    
    while (!game_snake.is_game_over()) {
        if(settings->speed_increase){
            Sleep(settings->snake_cur_speed);
            settings->change_cur_speed(0.97);
        }
        else{
            Sleep(settings->snake_init_speed);
        }
        if(settings->food_lifespan != 0)
            game_food.update_food_lifespans(game_map,game_snake);
        game_snake.move(game_map, game_food);
        game_map.render(game_snake, game_food);
    }
    cout << "Game Over! Final Score: " << game_snake.get_score() << endl;
}

void Game::starting()
{
    clear_all;
    cout<<"press SPACE bar to start, press S to view settings"<<endl;
    
    SHORT key;
    INPUT_RECORD input_record;
    DWORD writtten;
    HANDLE std_in_h = GetStdHandle(STD_INPUT_HANDLE);

    while(true){
        while(true){
            ReadConsoleInputA(std_in_h,&input_record,1,&writtten);
            if(input_record.EventType==KEY_EVENT&&input_record.Event.KeyEvent.bKeyDown) 
            {
                key=input_record.Event.KeyEvent.wVirtualScanCode;
#ifdef debug
                cout<<key;
#endif
                break;
            }
        }
        if(key==57) break;//space
        else if(key==31) {
            clear_all;
            settings->change_settings();
            clear_all;
            cout<<"settings saved!"<<endl;
            cout<<"press SPACE bar to start, press S to view settings"<<endl;
        }
    }
    
    return;
}
