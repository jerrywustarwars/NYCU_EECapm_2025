#include "lib/map.hpp"
#include "lib/game.hpp"
#include "lib/snake.hpp"
#include "lib/food.hpp"
#include "lib/key.hpp"
#include "lib/settings.hpp"
#include "lib/def.hpp"
#include <bits/stdc++.h>
#include <windows.h>
#include <codecvt>
using namespace std;

int main() {
#ifndef BUILT_IN
    system("chcp 65001"); //set console to UTF-8
#endif
    /*
    WARNING!!! 
    this can cause problems while using built-in terminal of text editors(vscode etc.)
    please comment it if you are using a built-in terminal
    or use g++ main.cpp lib/*.cpp -DBUILT_IN -o main_built_in
    */
    Settings *settings = new Settings(0,true,false,true,false,{50,30},6,3,12,750,6);
    while(true){
        Game *game = new Game(settings);
        delete game;
        cout<<"restart game? (y/n) ";
        char tmp; cin>>tmp;
        if(tmp == 'y') continue;
        else return 0;
    }
    return 0;
}

// g++ main.cpp lib/*.cpp -o main_terminal
