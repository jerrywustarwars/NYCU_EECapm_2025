#include "map.hpp"
#include "snake.hpp"
#include "food.hpp"
#include "key.hpp"
#include "def.hpp"
using namespace std;

KeyManager::KeyManager()
    :last_dir('w'), last_key('w'), start(true),dir_update(true){
    thread r_k(&KeyManager::update_status,this);
    r_k.detach();
}

KeyManager::~KeyManager(){
    start=false;
    INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 'w';

    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 'w';
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT unsent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
#ifdef DEBUG
    if (unsent != ARRAYSIZE(inputs))
    {
        cout<<"SendInput failed"<<HRESULT_FROM_WIN32(GetLastError());
    } 
    cout_line;
#endif
}

void KeyManager::start_game(){
    // thread r_k(&KeyManager::update_status,this);
    // r_k.detach();
}

void KeyManager::update_status(){
    char cur = 'w';
    last_dir=cur;
    last_key=cur;
    while(start){
        cur = read_key();
        dir_update=true;
        if(cur == 'w' || cur == 'a' || cur == 's' || cur == 'd')
        {
            last_dir=cur;
        }
        last_key=cur;   
    }
    return;
}


char KeyManager::read_key() {
    INPUT_RECORD input_record;
    DWORD writtten;
    HANDLE std_in_h = GetStdHandle(STD_INPUT_HANDLE);
    while(true)
    {
        ReadConsoleInputA(std_in_h,&input_record,1,&writtten);
        if(input_record.EventType==KEY_EVENT&&input_record.Event.KeyEvent.bKeyDown) break;
    }
    switch (input_record.Event.KeyEvent.wVirtualScanCode)
    {
    case 72: return 'w';
    case 75: return 'a';
    case 80: return 's';
    case 77: return 'd';
    default: return input_record.Event.KeyEvent.uChar.AsciiChar;
    }
}
/*
space 57
enter 28
esc 1
*/

void KeyManager::used_dir(){
    dir_update=false;
}
bool KeyManager::new_dir_input(){
    return dir_update;
}
