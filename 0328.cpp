#include <bits/stdc++.h>
#include <windows.h>
using namespace std;
// #define int long long
#define endl "\n"
#define pb push_back
#define f first
#define s second
#define all(x) x.begin(), x.end()
#define cout_line cout << "line" << __LINE__ << endl
#define cout_val(x) cout << #x << ": " << x << endl
#define cout cerr
#define clear_all cout << "\x1B[2J\x1B[H" << flush
#define io_speedup                \
    ios_base::sync_with_stdio(0); \
    cin.tie(0);
// #define DEBUG
// ─│┌┐└┘├┤┬┴┼╭╮╯╰○●★•∩∪⊂⊃

const int dir[5][2] ={{0,1},{1,0},{0,-1},{-1,0},{0,0}};//d,s,a,w,stop

struct keyManager
{
    char last_dir_c;
    int last_dir;
    std::map<char,int> wasd_to_dir={{'d',0},{'s',1},{'a',2},{'w',3}};
    bool start;
    keyManager() : last_dir_c('w'), last_dir(3), start(true)
    {
        thread r_k(&keyManager::update_status, this);
        r_k.detach();
    }
    ~keyManager()
    {
        start = false;
        INPUT inputs[2] = {};
        ZeroMemory(inputs, sizeof(inputs));

        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = 'w';

        inputs[2].type = INPUT_KEYBOARD;
        inputs[2].ki.wVk = 'w';
        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

        UINT unsent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    }
    void update_status()
    {
        char cur = 'w';
        last_dir_c = cur;

        while (start)
        {
            cur = read_key();
            if (cur == 'w' || cur == 'a' || cur == 's' || cur == 'd')
            {
                last_dir_c = cur;
                last_dir = wasd_to_dir[last_dir_c];
            }
        }
        return;
    }
    char read_key()
    {
        INPUT_RECORD input_record;
        DWORD writtten;
        HANDLE std_in_h = GetStdHandle(STD_INPUT_HANDLE);
        while (true)
        {
            ReadConsoleInputA(std_in_h, &input_record, 1, &writtten);
            if (input_record.EventType == KEY_EVENT &&
                input_record.Event.KeyEvent.bKeyDown)
                break;
        }
        switch (input_record.Event.KeyEvent.wVirtualScanCode)
        {
        case 72:
            return 'w';
        case 75:
            return 'a';
        case 80:
            return 's';
        case 77:
            return 'd';
        default:
            return input_record.Event.KeyEvent.uChar.AsciiChar;
        }
    }
    int get_last_key(){
        return last_dir;
    }
};

void map_print(string map[], int HEIGHT, int WIDTH)
{
    int n = HEIGHT, m = WIDTH;
    vector<vector<string>> map_display(n, vector<string>(m, " "));
    vector<vector<string>> map_display_color(n, vector<string>(m, ""));
    vector<vector<string>> map_display_thick(n, vector<string>(m, ""));

    function<string(string, string, string)> output_color = [&](string color, string thick, string output)
    {
        string ans = "\033[";
        std::map<string, string> color_map = {
            {"black", "0"},
            {"red", "196"},
            {"green", "46"},
            {"yellow", "11"},
            {"blue", "27"},
            {"purple", "105"},
            {"lightblue", "87"},
            {"white", "255"},
            {"pink", "219"},
            {"skin", "224"},
            {"orange", "214"}};
        std::map<string, string> thick_map = {
            {"bold", ";1"},
            {"normal", ""},
            {"light", ";2"}};
        ans += "38;5;" + color_map[color] + thick_map[thick] + "m";
        ans += output;
        ans += "\033[0m";
        return ans;
    };

    function<bool(int, int)> check_up = [&](int x, int y)
    {
        if (0 <= x && x < m && 0 <= y && x < n)
            if (map[x][y] == '#')
                return 1;
        return 0;
    };
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (map[i][j] == '#')
            {
                // 當前位置只看上下左右有幾個#
                //  一個:─or│   //兩個:─│┌┐└┘
                //  三個:看缺少的邊對馬步的兩個角
                //  有一個為空就是├┤┬┴  都是滿的就是─│
                //  四個:根據缺角
                //  缺一個┌┐└┘  缺兩個相鄰 ├┤┬┴ 缺兩個對角 ┼  缺三個以上 ┼
                map_display_color[i][j] = "blue";
                map_display_thick[i][j] = "normal";
                string type[16] = {"X", "─", "│", "╮", "─", "─", "╭", "┬", "│", "╯", "│", "┤", "╰", "┴", "├", "┼"}; // ─│┌┐└┘├┤┬┴┼
                // string type[16] = {"X", "─", "│", "┐", "─", "─", "┌", "┬", "│", "┘", "│", "┤", "└", "┴", "├", "┼"}; // ─│┌┐└┘├┤┬┴┼
                int next = check_up(i + 1, j) + check_up(i - 1, j) + check_up(i, j + 1) + check_up(i, j - 1);
                if (next == 1 || next == 2)
                {
                    int i_check = check_up(i - 1, j) * 8 + // w
                                  check_up(i, j + 1) * 4 + // d
                                  check_up(i + 1, j) * 2 + // s
                                  check_up(i, j - 1);      // a
                    map_display[i][j] = type[i_check];
                }
                else if (next == 3)
                {
                    if (!check_up(i + 1, j) && (check_up(i - 1, j + 1) && check_up(i - 1, j - 1)))
                        map_display[i][j] = type[1];
                    else if (!check_up(i - 1, j) && (check_up(i + 1, j + 1) && check_up(i + 1, j - 1)))
                        map_display[i][j] = type[1];
                    else if (!check_up(i, j + 1) && (check_up(i - 1, j - 1) && check_up(i + 1, j - 1)))
                        map_display[i][j] = type[2];
                    else if (!check_up(i, j - 1) && (check_up(i - 1, j + 1) && check_up(i + 1, j + 1)))
                        map_display[i][j] = type[2];
                    else
                    {
                        int i_check = check_up(i - 1, j) * 8 + // w
                                      check_up(i, j + 1) * 4 + // d
                                      check_up(i + 1, j) * 2 + // s
                                      check_up(i, j - 1);      // a

                        map_display[i][j] = type[i_check];
                    }
                }
                else if (next == 4)
                {
                    int angle = check_up(i + 1, j + 1) + check_up(i - 1, j + 1) + check_up(i + 1, j - 1) + check_up(i - 1, j - 1);
                    if (angle == 4)
                    {
                        map_display[i][j] = " ";
                    }
                    else if (angle == 3)
                    {
                        if (!check_up(i + 1, j + 1))
                            map_display[i][j] = type[6]; // ┌
                        else if (!check_up(i - 1, j + 1))
                            map_display[i][j] = type[12]; // └
                        else if (!check_up(i + 1, j - 1))
                            map_display[i][j] = type[3]; // ┐
                        else if (!check_up(i - 1, j - 1))
                            map_display[i][j] = type[9]; // ┘
                    }
                    else if (angle == 2)
                    {
                        if (!check_up(i + 1, j + 1) && !check_up(i + 1, j - 1))
                            map_display[i][j] = type[13]; // ┴
                        else if (!check_up(i - 1, j + 1) && !check_up(i - 1, j - 1))
                            map_display[i][j] = type[7]; // ┬
                        else if (!check_up(i + 1, j + 1) && !check_up(i - 1, j + 1))
                            map_display[i][j] = type[11]; // ┤
                        else if (!check_up(i + 1, j - 1) && !check_up(i - 1, j - 1))
                            map_display[i][j] = type[14]; // ├
                        else
                            map_display[i][j] = type[15]; // ┼
                    }
                    else
                        map_display[i][j] = type[15]; // ┼
                }
            }
            else if (map[i][j] == '0' || map[i][j] == '1' ||
                     map[i][j] == '2' || map[i][j] == '3')
            {
                map_display_color[i][j] = "yellow";
                map_display_thick[i][j] = "bold";
                // string type[4]={"⊂","∩","⊃","∪"};//∩∪⊂⊃
                map_display[i][j] = "●";
            }
            else if (map[i][j] == '*')
            {
                map_display_color[i][j] = "skin";
                map_display_thick[i][j] = "normal";
                map_display[i][j] = "○";
            }
            else if (map[i][j] == '.')
            {
                map_display_color[i][j] = "skin";
                map_display_thick[i][j] = "normal";
                map_display[i][j] = ".";
            }
            else if (map[i][j] == ' ')
            {
                map_display_color[i][j] = "white";
                map_display_thick[i][j] = "normal";
                map_display[i][j] = " ";
            }
            else if (isalpha(map[i][j]))
            {
                if (isupper(map[i][j]))
                {
                    map_display_color[i][j] = "purple";
                    map_display_thick[i][j] = "normal";
                    map_display[i][j] = "ö";
                }
                else if (islower(map[i][j]))
                {
                    string ghost[4] = {"red", "pink", "lightblue", "orange"};
                    map_display_color[i][j] = ghost[map[i][j] - 'a'];
                    map_display_thick[i][j] = "normal";
                    map_display[i][j] = "∩"; // öё
                }
            }
            cout << output_color(map_display_color[i][j], map_display_thick[i][j], map_display[i][j]);
        }
        cout << endl;
    }

    // for(int i=0;i<n;i++){
    //     for(int j=0;j<m;j++){
    //         cout<<output_color(map_display_color[i][j],"normal",map_display[i][j]);
    //     }
    //     cout<<endl;
    // }
}

bool check_not_collide_wall(string map[], int i, int j){
    int n=sizeof(map),m=map[0].size();
    if(i<0||i>=n||j<0||j>=m||map[i][j]=='#') return false;
    return true;
}

int best_dir(string map[] ,int cur_pos[2], int cur_dir, int target[2]){
    int n=sizeof(map),m=map[0].size();
    int ans = 4;
    int min_dis=INT16_MAX;
    for(int i=0;i<4;i++){
        if(i == (cur_dir+2)%4) continue;
        int dis =(cur_pos[0]+dir[i][0])*(cur_pos[0]+dir[i][0])+(cur_pos[1]+dir[i][1])*(cur_pos[1]+dir[i][1]);
        if(dis<min_dis && check_not_collide_wall(map,cur_pos[0]+dir[i][0],cur_pos[1]+dir[i][1])){
            ans = i;
            min_dis = dis;
        }
    }
    return ans;
}

void move_pacman(string map[], int (&player_pos)[2],int &cur_dir,keyManager &keyM){
    int input = keyM.get_last_key();
    if(check_not_collide_wall(map,player_pos[0]+dir[input][0],player_pos[1]+dir[input][1]))
    {
        player_pos[0]+=dir[input][0];
        player_pos[1]+=dir[input][1];
        cur_dir = input;
    }
    else{
        cur_dir = 4;
    }
    return;
}

bool check_collide_ghost(int (*ghost_pos)[2], int player_pos[2],int ghost_num){
    for(int i=0;i<ghost_num;i++){
        if(ghost_pos[i][0]==player_pos[0] && ghost_pos[i][1]
            return false;
                                                //todo
}


int main()
{
    int WIDTH = 20;
    int HEIGHT = 10;
    string init_grid[HEIGHT] = {
        "####################",
        "#..a..........0....#",
        "#..####.#######.#..#",
        "#............d..#..#",
        "#..###.###.###.#####",
        "#..................#",
        "####.###.#.###.###.#",
        "#.c..........b..#..#",
        "#..###########.....#",
        "####################"};
    const int power_num = 4, ghost_num = 4;
    int power_cnt = 0, ghost_cnt = 0;
    int points_cnt;

    int player_pos[2];
    int player_dir = 3; //w

    int ghost_pos[ghost_num][2];
    int ghost_dir[ghost_num]={};
    char ghost_type[ghost_num];

    int score = 0;
    int frighten_time;

    char grid_cur[HEIGHT][WIDTH], grid_next[HEIGHT][WIDTH];
    char points_map[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++) //設定player_pos,ghost_pos,points_map等的初始狀態
    {
        for(int j=0;j<WIDTH;j++){
            char c = init_grid[i][j];
            grid_cur[i][j] = c;
            grid_next[i][j] = c;
            
            if(c == '.' ||c == '*')
            {
                points_map[i][j]=c;
                points_cnt++;
            }
            if(c == '0'){
                player_pos[0] = i;
                player_pos[1] = j;
            }
            if(isalpha(c)){
                ghost_pos[ghost_cnt][0] = i;
                ghost_pos[ghost_cnt][1] = j;
                ghost_type[ghost_cnt] = c;
                ghost_cnt++;
            }
        }
    }
    const int points_num = points_cnt;

    //game start
    keyManager keyM;
    int cur_dir;
    while (true){
        move_pacman(init_grid,player_pos,player_dir,keyM);

        
    }

    //red curpos
    //pink 4tiles
    //lightblue anti-red or 2tiles forward
    //orange keepdis
}