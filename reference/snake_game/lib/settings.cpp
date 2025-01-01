#include "settings.hpp"
#include "def.hpp"
using namespace std;

Settings::Settings(int _gm, bool _bwa, bool _si, bool _rs, bool _npu, Coord _ms, int _fd, int _wd, int _fl, int _ss, int _nput)
    : game_mode(_gm),
      boundary_wrap_around(_bwa),
      speed_increase(_si),
      rand_start(_rs),
      negative_power_up(_npu),
      map_size(_ms),
      food_dense(_fd),
      wall_dense(_wd),
      food_lifespan(_fl),
      snake_init_speed(_ss),
      snake_cur_speed(_ss),
      init_negative_power_up_time(_nput),
      cur_negative_power_up_time(0)
{
    double tmp = map_size.X * map_size.Y * food_dense / 100;
    food_cnt = round(tmp);
    update_setting_display();
}

void Settings::change_settings()
{
    cout << "Settings:" << endl;
    for (int i = 0; i < setting_display.size(); i++)
    {
        cout << i+1 << ". " << setting_display[i][0] << ": " << setting_display[i][1] << endl;
    }
    cout << "Enter the number of the setting you want to change, enter 0 to exit: ";
    int choice;
    cin >> choice;
    while (choice < 0 || choice > setting_display.size())
    {
        cout << "Invalid choice. Please enter a number between 0 and " << setting_display.size() << ": ";
        cin >> choice;
    }
    if (choice == 0)
        return;
    choice--;
    cout << "Modifying " << setting_display[choice][0] << " " << setting_display[choice][2] << ": ";
    string tmp;
    cin >> tmp;
#ifdef DEBUG
    cout_val(tmp);
    Sleep(1000);
#endif
    switch (choice)
    {
        case 0:{
            if (tmp == "easy"){
                game_mode = 0;snake_init_speed = 750;wall_dense = 3;food_dense = 6;
#ifdef DEBUG
                cout_line;
#endif
            }
            else if (tmp == "medium"){
                game_mode = 1;snake_init_speed = 500;wall_dense = 4;food_dense = 5;
#ifdef DEBUG
                cout_line;
#endif
            }
            else if (tmp == "hard"){
                game_mode = 2;snake_init_speed = 250;wall_dense = 6;food_dense = 3;
#ifdef DEBUG
                cout_line;
#endif
            }
        }break;
        
        case 1 ... 4:{
            bool* ptr_b[] = {
                &boundary_wrap_around,
                &speed_increase,
                &rand_start,
                &negative_power_up
            };
            if (tmp == "on") *ptr_b[choice-1]=true;
            else if(tmp == "off") *ptr_b[choice-1]=false;
#ifdef DEBUG
            cout_line;
#endif
        }break;

        case 5 ... 11:{
            int* ptr_i[] = {
                &map_size.X,
                &map_size.Y,
                &food_dense,
                &wall_dense,
                &food_lifespan,
                &snake_init_speed,
                &init_negative_power_up_time
            };
            *ptr_i[choice-5]=stoi(tmp);
#ifdef DEBUG
            cout_line;
#endif
        }break;
    }
#ifdef DEBUG
        Sleep(1000);
#endif
    if(snake_init_speed<200) snake_init_speed=200;
    snake_cur_speed = snake_init_speed;
    cur_negative_power_up_time = init_negative_power_up_time;
    update_setting_display();
    clear_all;
    cout << "Settings saved!" << endl;
    change_settings();
    return;
}

void Settings::change_cur_speed(double a){
    double tmp = snake_cur_speed * a;
    snake_cur_speed = round(tmp);
    if(snake_cur_speed<200) snake_cur_speed = 200;
    return;
}

void Settings::update_setting_display()
{
    function<string(bool)> get_state = [&](bool a)
    {
        return a ? "on" : "off";
    };
    setting_display = {
        {"Game mode", game_mode_name[game_mode], "(easy/medium/hard, WARNING changing this will modify snake speed, obstacle density and food lifespan as well)"},
        {"Boundary wrap around", get_state(boundary_wrap_around), "(on/off)"},
        {"Speed Increase", get_state(speed_increase), "(on/off)"},
        {"Randomized Initial Positions", get_state(rand_start), "(on/off)"},
        {"Negative power up", get_state(negative_power_up), "(on/off)"},
        {"Map width", to_string(map_size.X), ""},
        {"Map height", to_string(map_size.Y), ""},
        {"Food density", to_string(food_dense), "(precentage of the map area, recommend 3~7)"},
        {"Wall density", to_string(wall_dense), "(precentage of the map area, recommend 3~7)"},
        {"Food lifespan", to_string(food_lifespan), "(in seconds, will start blinking in the last three seconds, set to 0 to let food won't dissapear)"},
        {"Snake speed", to_string(snake_init_speed), "(milliseconds between move, at least 200)"},
        {"Negative power up time", to_string(init_negative_power_up_time), "(number of frame)"}
    };
}

void Settings::change_cur_negative_power_up_time(int a){
    cur_negative_power_up_time = a;
    return;
}