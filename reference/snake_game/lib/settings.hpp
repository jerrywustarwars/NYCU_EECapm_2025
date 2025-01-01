#pragma once
#include <bits/stdc++.h>
#include <windows.h>
#include "def.hpp"

using namespace std;

class Settings{
public:
    Settings(int _gm, bool _bwa, bool _si, bool _rs, bool _npu, Coord _ms, int _fd, int _wd, int _fl, int _ss ,int _nput);
    bool boundary_wrap_around,
         speed_increase,
         rand_start,
         negative_power_up;
    Coord map_size;
    int game_mode,
        food_cnt, 
        food_dense, 
        wall_dense,
        food_lifespan,
        snake_init_speed,
        snake_cur_speed,
        init_negative_power_up_time,
        cur_negative_power_up_time;
    
    void change_settings();
    void change_cur_speed(double a);
    void change_cur_negative_power_up_time(int a);
private:
    vector<vector<string>> setting_display;
    void update_setting_display();
    const string game_mode_name[3]={"easy","medium","hard"};
};