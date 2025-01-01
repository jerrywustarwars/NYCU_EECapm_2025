#pragma once

#include <bits/stdc++.h>
#include <windows.h>
#include <winuser.h>
#include "map.hpp"
#include "snake.hpp"
#include "food.hpp"
#include "key.hpp"
#include "def.hpp"
using namespace std;

class KeyManager {
public:
    KeyManager();
    ~KeyManager();
    void start_game();
    void update_status();
    char read_key();
    char last_dir;
    char last_key;
    void used_dir();
    bool new_dir_input();
private:
    bool start;
    bool dir_update;
};


