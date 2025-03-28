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



void map_print(string map[], int HEIGHT, int WIDTH)
{
    int n = HEIGHT, m = WIDTH;
    vector<vector<string>> map_display(n, vector<string>(m, " "));
    vector<vector<string>> map_display_color(n, vector<string>(m, ""));
    vector<vector<string>> map_display_thick(n, vector<string>(m, ""));
    
    function<string(string,string,string)> output_color = [&](string color, string thick, string output){
        string ans = "\033[";
        std::map<string,string> color_map = {
            {"black","0"},
            {"red","196"},
            {"green","46"},
            {"yellow","11"},
            {"blue","27"},
            {"purple","105"},
            {"lightblue","87"},
            {"white","255"},
            {"pink","219"},
            {"skin","224"},
            {"orange","214"}
        };
        std::map<string,string> thick_map ={
            {"bold", ";1"},
            {"normal", ""},
            {"light", ";2"}
        };
        ans+= "38;5;"+ color_map[color] + thick_map[thick] + "m";
        ans+= output;
        ans+= "\033[0m";
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
                //當前位置只看上下左右有幾個#
                // 一個:─or│   //兩個:─│┌┐└┘
                // 三個:看缺少的邊對馬步的兩個角
                // 有一個為空就是├┤┬┴  都是滿的就是─│
                // 四個:根據缺角
                // 缺一個┌┐└┘  缺兩個相鄰 ├┤┬┴ 缺兩個對角 ┼  缺三個以上 ┼
                map_display_color[i][j] = "blue";
                map_display_thick[i][j] = "normal";
                string type[16] = {"X", "─", "│", "╮", "─", "─", "╭", "┬", "│", "╯", "│", "┤", "╰", "┴", "├", "┼"}; // ─│┌┐└┘├┤┬┴┼
                //string type[16] = {"X", "─", "│", "┐", "─", "─", "┌", "┬", "│", "┘", "│", "┤", "└", "┴", "├", "┼"}; // ─│┌┐└┘├┤┬┴┼
                int next = check_up(i + 1, j) + check_up(i - 1, j) + check_up(i, j + 1) + check_up(i, j - 1);
                if (next == 1 || next == 2)
                {
                    int i_check = check_up(i - 1, j) * 8 +                                                              // w
                                  check_up(i, j + 1) * 4 +                                                              // d
                                  check_up(i + 1, j) * 2 +                                                              // s
                                  check_up(i, j - 1);                                                                   // a
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
                        int i_check = check_up(i - 1, j) * 8 +                                                              // w
                                      check_up(i, j + 1) * 4 +                                                              // d
                                      check_up(i + 1, j) * 2 +                                                              // s
                                      check_up(i, j - 1);                                                                   // a
                        
                        map_display[i][j] = type[i_check];
                    }
                }
                else if (next == 4)
                {
                    int angle = check_up(i + 1, j + 1) + check_up(i - 1, j + 1) + check_up(i + 1, j - 1) + check_up(i - 1, j - 1);
                    if (angle == 4){
                        map_display[i][j] = " ";
                    }
                    else if (angle == 3)
                    {
                        if (!check_up(i + 1, j + 1))
                            map_display[i][j] = type[6];//┌
                        else if (!check_up(i - 1, j + 1))
                            map_display[i][j] = type[12];//└
                        else if (!check_up(i + 1, j - 1))
                            map_display[i][j] = type[3];//┐
                        else if (!check_up(i - 1, j - 1))
                            map_display[i][j] = type[9];//┘
                    }
                    else if (angle == 2)
                    {
                        if (!check_up(i + 1, j + 1) && !check_up(i + 1, j - 1))
                            map_display[i][j] = type[13];//┴
                        else if (!check_up(i - 1, j + 1) && !check_up(i - 1, j - 1))
                            map_display[i][j] = type[7];//┬
                        else if (!check_up(i + 1, j + 1) && !check_up(i - 1, j + 1))
                            map_display[i][j] = type[11];//┤
                        else if (!check_up(i + 1, j - 1) && !check_up(i - 1, j - 1))
                            map_display[i][j] = type[14];//├
                        else map_display[i][j] = type[15];//┼
                    }
                    else map_display[i][j] = type[15];//┼
                }
            }
            else if(map[i][j] == '0' || map[i][j] == '1' ||
                    map[i][j] == '2' || map[i][j] == '3'){
                map_display_color[i][j] = "yellow";
                map_display_thick[i][j] = "bold";
                //string type[4]={"⊂","∩","⊃","∪"};//∩∪⊂⊃
                map_display[i][j] = "●";
            }
            else if(map[i][j] == '*'){
                map_display_color[i][j] = "skin";
                map_display_thick[i][j] = "normal";
                map_display[i][j] = "○";
            }
            else if(map[i][j] == '.'){
                map_display_color[i][j] = "skin";
                map_display_thick[i][j] = "normal";
                map_display[i][j] = ".";
            }
            else if(map[i][j]== ' '){
                map_display_color[i][j] = "white";
                map_display_thick[i][j] = "normal";
                map_display[i][j] = " ";
            }
            else if(isalpha(map[i][j])){
                if(isupper(map[i][j])){
                    map_display_color[i][j] = "purple";
                    map_display_thick[i][j] = "normal";
                    map_display[i][j] = "ö";
                }
                else if(islower(map[i][j])){
                    string ghost[4]={"red", "pink", "lightblue", "orange"};
                    map_display_color[i][j] = ghost[map[i][j]-'a'];
                    map_display_thick[i][j] = "normal";
                    map_display[i][j] = "∩";//öё
                }
                
            }
            cout<<output_color(map_display_color[i][j],map_display_thick[i][j],map_display[i][j]);
        }
        cout<<endl;
    }
    
    // for(int i=0;i<n;i++){
    //     for(int j=0;j<m;j++){
    //         cout<<output_color(map_display_color[i][j],"normal",map_display[i][j]);
    //     }
    //     cout<<endl;
    // }
}



/*
void map_print(string map[], int HEIGHT, int WIDTH)
{
    int n = HEIGHT, m = WIDTH;
    vector<vector<string>> map_display(n, vector<string>(m, " "));
    vector<vector<string>> map_display_color(n, vector<string>(m, ""));
    vector<vector<string>> map_display_thick(n, vector<string>(m, ""));
    
    function<string(string,string,string)> output_color = [&](string color, string thick, string output){
        string ans = "\033[";
        std::map<string,string> color_map = {
            {"black","0"},
            {"red","196"},
            {"green","46"},
            {"yellow","11"},
            {"blue","27"},
            {"purple","105"},
            {"lightblue","87"},
            {"white","255"},
            {"pink","219"},
            {"skin","224"},
            {"orange","214"}
        };
        std::map<string,string> thick_map ={
            {"bold", ";1"},
            {"normal", ""},
            {"light", ";2"}
        };
        ans+= "38;5;"+ color_map[color] + thick_map[thick] + "m";
        ans+= output;
        ans+= "\033[0m";
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
                //當前位置只看上下左右有幾個#
                // 一個:─or│   //兩個:─│┌┐└┘
                // 三個:看缺少的邊對馬步的兩個角
                // 有一個為空就是├┤┬┴  都是滿的就是─│
                // 四個:根據缺角
                // 缺一個┌┐└┘  缺兩個相鄰 ├┤┬┴ 缺兩個對角 ┼  缺三個以上 ┼
                map_display_color[i][j] = "blue";
                map_display_thick[i][j] = "normal";
                string type[16] = {"X", "─", "│", "╮", "─", "─", "╭", "┬", "│", "╯", "│", "┤", "╰", "┴", "├", "┼"}; // ─│┌┐└┘├┤┬┴┼
                //string type[16] = {"X", "─", "│", "┐", "─", "─", "┌", "┬", "│", "┘", "│", "┤", "└", "┴", "├", "┼"}; // ─│┌┐└┘├┤┬┴┼
                int next = check_up(i + 1, j) + check_up(i - 1, j) + check_up(i, j + 1) + check_up(i, j - 1);
                if (next == 1 || next == 2)
                {
                    int i_check = check_up(i - 1, j) * 8 +                                                              // w
                                  check_up(i, j + 1) * 4 +                                                              // d
                                  check_up(i + 1, j) * 2 +                                                              // s
                                  check_up(i, j - 1);                                                                   // a
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
                        int i_check = check_up(i - 1, j) * 8 +                                                              // w
                                      check_up(i, j + 1) * 4 +                                                              // d
                                      check_up(i + 1, j) * 2 +                                                              // s
                                      check_up(i, j - 1);                                                                   // a
                        
                        map_display[i][j] = type[i_check];
                    }
                }
                else if (next == 4)
                {
                    int angle = check_up(i + 1, j + 1) + check_up(i - 1, j + 1) + check_up(i + 1, j - 1) + check_up(i - 1, j - 1);
                    if (angle == 4){
                        map_display[i][j] = " ";
                    }
                    else if (angle == 3)
                    {
                        if (!check_up(i + 1, j + 1))
                            map_display[i][j] = type[6];//┌
                        else if (!check_up(i - 1, j + 1))
                            map_display[i][j] = type[12];//└
                        else if (!check_up(i + 1, j - 1))
                            map_display[i][j] = type[3];//┐
                        else if (!check_up(i - 1, j - 1))
                            map_display[i][j] = type[9];//┘
                    }
                    else if (angle == 2)
                    {
                        if (!check_up(i + 1, j + 1) && !check_up(i + 1, j - 1))
                            map_display[i][j] = type[13];//┴
                        else if (!check_up(i - 1, j + 1) && !check_up(i - 1, j - 1))
                            map_display[i][j] = type[7];//┬
                        else if (!check_up(i + 1, j + 1) && !check_up(i - 1, j + 1))
                            map_display[i][j] = type[11];//┤
                        else if (!check_up(i + 1, j - 1) && !check_up(i - 1, j - 1))
                            map_display[i][j] = type[14];//├
                        else map_display[i][j] = type[15];//┼
                    }
                    else map_display[i][j] = type[15];//┼
                }
            }
            else if(map[i][j] == '0' || map[i][j] == '1' ||
                    map[i][j] == '2' || map[i][j] == '3'){
                map_display_color[i][j] = "yellow";
                map_display_thick[i][j] = "normal";
                string type[4]={"⊂","∩","⊃","∪"};//∩∪⊂⊃
                map_display[i][j] = type[map[i][j] - '0'];
            }
            else if(map[i][j] == '*'){
                map_display_color[i][j] = "skin";
                map_display[i][j] = "○";
            }
            else if(map[i][j] == '.'){
                map_display_color[i][j] = "skin";
                map_display[i][j] = "•";
            }
            else if(map[i][j]== ' '){
                map_display_color[i][j] = "white";
                map_display[i][j] = " ";
            }
            else if(isalpha(map[i][j])){
                if(isupper(map[i][j])){
                    map_display_color[i][j] = "purple";
                    map_display[i][j] = "ё";
                }
                else if(islower(map[i][j])){
                    string ghost[4]={"red", "pink", "lightblue", "orange"};
                    map_display_color[i][j] = ghost[map[i][j]-'a'];
                    map_display[i][j] = "ö";//öё
                }
                
            }
            cout<<output_color(map_display_color[i][j],"normal",map_display[i][j]);
        }
        cout<<endl;
    }
    
    // for(int i=0;i<n;i++){
    //     for(int j=0;j<m;j++){
    //         cout<<output_color(map_display_color[i][j],"normal",map_display[i][j]);
    //     }
    //     cout<<endl;
    // }
}
*/
int main() {
    string matrix[] = {
        "####################",
        "#...*..*.. 0.1.2.3.#",
        "#..####.#######.#..#",
        "#.a.....###.B...#..#",
        "#..###.#######.#####",
        "#.....b.A..###...###",
        "####.###.#.###.#####",
        "#....###...c....#..#",
        "#..###########...d.#",
        "####################",
        "####################"
    };
    
    map_print(matrix,11,20);
    return 0;
}


// ####################
// #..................#
// #..####.#######.#..#
// #.......###.....#..#
// #..###.#######.#####
// #..........###...###
// ####.###.#.###.#####
// #....###........#..#
// #..###########.....#
// ####################
// ####################

// @@@@@@@@@@@@@@@@@@@@
// @..................@
// @..@@@@.@@@@@@@.@..@
// @.......@#@.....@..@
// @..@@@.@@@@@@@.@@@@@
// @..........@#@...@#@
// @@@@.@@@.@.@@@.@@@@@
// @....@#@........@..@
// @..@@@#@@@@@@@.....@
// @@@@#########@@@@@@@
// @@@@@@@@@@@@@@@@@@@@


// .......####........
// .......####........
// ..############.....
// ..#########........
// .......####........

// .......┌──┐........
// .......│  │........
// ..┌────┘  ├───.....
// ..└────┐  │........
// .......└──┘........

// .......┌┬┬┐........
// .......├┼┼┤........
// ..┌┬┬┬┬┼┼┼┼───.....
// ..└┴┴┴┴┼┼┼┤........
// .......└┴┴┘........

//........
//...┌┐...
//.┌─┼┴─..
//.│.│....
//........


//─│┌┐└┘├┤┬┴┼



// 當前位置只看上下左右有幾個井字
// 一個:─or│
// 兩個:─│┌┐└┘

// 三個:
// 看缺少的邊對馬步的兩個角
//  有一個為空就是├┤┬┴
// 都是滿的就是─│

// 四個
// 根據缺角
// 缺一個
// ┌┐└┘
// 兩個
// ├┤┬┴ 相鄰
// ┼對角

// 缺三個以上
// ┼

