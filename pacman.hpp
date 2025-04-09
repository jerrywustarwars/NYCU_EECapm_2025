#pragma once
#include <bits/stdc++.h>
#include <windows.h>
using namespace std;


extern int dir[5][2]; 
extern int WIDTH, HEIGHT;
extern bool gameRunning;

bool checkNotCollideWall(string grid[], int i, int j);
bool checkCollideGhost(int (*ghostPos)[2], int playerPos[2], int ghostNum);
int bestDir(string grid[], int curPos[2], int curDir, int target[2]);
template <int _N>
void moveGhost(string grid[], int (&ghostPos)[_N][2], int (&ghostDir)[_N],
                char ghostType[], int ghostNum, int playerPos[2],
                int playerDir, bool scaredMode);
void eatGhost(string grid[], int playerPos[2], int (*ghostPos)[2],
                int (&ghostDir)[], char (&ghostType)[], int ghostNum,
                int &score);

// 鍵盤管理器結構
struct KeyManager {
    char lastDirC;  // 最後按下的方向鍵字符
    int lastDir;    // 最後按下的方向
    std::map<char, int> wasdToDir = {{'d', 0}, {'s', 1}, {'a', 2}, {'w', 3}};  // WASD鍵對應的方向
    bool start;  // 是否開始
    bool newInput;  // 是否有新輸入

    KeyManager() : lastDirC('w'), lastDir(4), start(true), newInput(false) {
        thread rK(&KeyManager::updateStatus, this);  // 啟動更新狀態的線程
        rK.detach();  // 分離線程
    }

    ~KeyManager() {
        start = false;  // 停止更新狀態
        INPUT inputs[3] = {};
        ZeroMemory(inputs, sizeof(inputs));  // 清空輸入

        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = 'w';  // 模擬按下'w'鍵

        inputs[2].type = INPUT_KEYBOARD;
        inputs[2].ki.wVk = 'w';
        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;  // 模擬釋放'w'鍵

        UINT unsent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));  // 發送輸入
    }

    // 更新鍵盤狀態
    void updateStatus() {
        char cur = 'w';
        lastDirC = cur;

        while (start) {
            cur = readKey();  // 讀取當前按鍵
            newInput = true;
            if (cur == 'w' || cur == 'a' || cur == 's' || cur == 'd') {
                lastDirC = cur;
                lastDir = wasdToDir[lastDirC];  // 更新方向
            }
        }
        return;
    }

    // 讀取按鍵
    char readKey() {
        INPUT_RECORD inputRecord;
        DWORD written;
        HANDLE stdInH = GetStdHandle(STD_INPUT_HANDLE);
        while (true) {
            ReadConsoleInputA(stdInH, &inputRecord, 1, &written);  // 讀取控制台輸入
            if (inputRecord.EventType == KEY_EVENT &&
                inputRecord.Event.KeyEvent.bKeyDown)
                break;  // 如果是按鍵事件且按鍵被按下，則退出循環
        }
        switch (inputRecord.Event.KeyEvent.wVirtualScanCode) {
            case 72:
                return 'w';  // 回傳'w'
            case 75:
                return 'a';  // 回傳'a'
            case 80:
                return 's';  // 回傳's'
            case 77:
                return 'd';  // 回傳'd'
            default:
                return inputRecord.Event.KeyEvent.uChar.AsciiChar;  // 回傳其他字符
        }
    }

    // 獲取最後按下的鍵
    int getLastKey() {
        return lastDir;
    }

    void anyKeyToContinue() {
        newInput = false;
        while (!newInput) {}
        return;
    }
};

// 輸出遊戲網格
void gridPrint(string grid[], string (&prevGrid)[]) {
    int n = HEIGHT, m = WIDTH;
    vector<vector<string>> gridDisplay(n, vector<string>(m, " "));
    vector<vector<string>> gridDisplayColor(n, vector<string>(m, ""));
    vector<vector<string>> gridDisplayThick(n, vector<string>(m, ""));

    // 輸出顏色的函數
    function<string(string, string, string)> outputColor =
        [&](string color, string thick, string output) {
            string ans = "\033[";
            std::map<string, string> colorGrid = {
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
            std::map<string, string> thickGrid = {
                {"bold", ";1"},
                {"normal", ""},
                {"light", ";2"}};
            ans += "38;5;" + colorGrid[color] + thickGrid[thick] + "m";
            ans += output;
            ans += "\033[0m";
            return ans;
        };

    // 檢查上方是否有牆壁
    function<bool(int, int)> checkUp = [&](int x, int y) {
        if (0 <= x && x < n && 0 <= y && y < m)
            if (grid[x][y] == '#')
                return 1;  // 如果是牆壁，回傳1
        return 0;  // 否則回傳0
    };

    // 填充網格顯示
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == '#') {

                //當前位置只看上下左右有幾個#
                // 一個:─or│   //兩個:─│┌┐└┘
                // 三個:看缺少的邊對馬步的兩個角
                // 有一個為空就是├┤┬┴  都是滿的就是─│
                // 四個:根據缺角
                // 缺一個┌┐└┘  缺兩個相鄰 ├┤┬┴ 缺兩個對角 ┼  缺三個以上 ┼
                
                gridDisplayColor[i][j] = "blue";  // 牆壁顏色
                gridDisplayThick[i][j] = "normal";  // 牆壁粗細
                string type[16] = {"X", "─", "│", "╮", "─", "─", "╭", "┬", "│", "╯", "│", "┤", "╰", "┴", "├", "┼"};
                int next = checkUp(i + 1, j) + checkUp(i - 1, j) +
                           checkUp(i, j + 1) + checkUp(i, j - 1);
                if (next == 1 || next == 2) {
                    int iCheck = checkUp(i - 1, j) * 8 +
                                 checkUp(i, j + 1) * 4 +
                                 checkUp(i + 1, j) * 2 +
                                 checkUp(i, j - 1);
                    gridDisplay[i][j] = type[iCheck];  // 根據周圍牆壁的數量選擇顯示的字符
                } else if (next == 3) {
                    if (!checkUp(i + 1, j) &&
                        (checkUp(i - 1, j + 1) && checkUp(i - 1, j - 1)))
                        gridDisplay[i][j] = type[1];
                    else if (!checkUp(i - 1, j) &&
                             (checkUp(i + 1, j + 1) && checkUp(i + 1, j - 1)))
                        gridDisplay[i][j] = type[1];
                    else if (!checkUp(i, j + 1) &&
                             (checkUp(i - 1, j - 1) && checkUp(i + 1, j - 1)))
                        gridDisplay[i][j] = type[2];
                    else if (!checkUp(i, j - 1) &&
                             (checkUp(i - 1, j + 1) && checkUp(i + 1, j + 1)))
                        gridDisplay[i][j] = type[2];
                    else {
                        int iCheck = checkUp(i - 1, j) * 8 +
                                     checkUp(i, j + 1) * 4 +
                                     checkUp(i + 1, j) * 2 +
                                     checkUp(i, j - 1);
                        gridDisplay[i][j] = type[iCheck];
                    }
                } else if (next == 4) {
                    int angle = checkUp(i + 1, j + 1) + checkUp(i - 1, j + 1) +
                                checkUp(i + 1, j - 1) + checkUp(i - 1, j - 1);
                    if (angle == 4) {
                        gridDisplay[i][j] = " ";  // 如果四周都有牆，顯示空格
                    } else if (angle == 3) {
                        if (!checkUp(i + 1, j + 1))
                            gridDisplay[i][j] = type[6];  // ┌
                        else if (!checkUp(i - 1, j + 1))
                            gridDisplay[i][j] = type[12]; // └
                        else if (!checkUp(i + 1, j - 1))
                            gridDisplay[i][j] = type[3];  // ┐
                        else if (!checkUp(i - 1, j - 1))
                            gridDisplay[i][j] = type[9];  // ┘
                    } else if (angle == 2) {
                        if (!checkUp(i + 1, j + 1) && !checkUp(i + 1, j - 1))
                            gridDisplay[i][j] = type[13]; // ┴
                        else if (!checkUp(i - 1, j + 1) && !checkUp(i - 1, j - 1))
                            gridDisplay[i][j] = type[7];  // ┬
                        else if (!checkUp(i + 1, j + 1) && !checkUp(i - 1, j + 1))
                            gridDisplay[i][j] = type[11]; // ┤
                        else if (!checkUp(i + 1, j - 1) && !checkUp(i - 1, j - 1))
                            gridDisplay[i][j] = type[14]; // ├
                        else
                            gridDisplay[i][j] = type[15]; // ┼
                    } else
                        gridDisplay[i][j] = type[15]; // ┼
                }
            } else if (grid[i][j] == '0') {
                gridDisplayColor[i][j] = "yellow";  // pacman顏色
                gridDisplayThick[i][j] = "bold";  // pacman粗細
                gridDisplay[i][j] = "●";  // pacman符號
            } else if (grid[i][j] == '*') {
                gridDisplayColor[i][j] = "skin";  // 能量球顏色
                gridDisplayThick[i][j] = "normal";  // 能量球粗細
                gridDisplay[i][j] = "○";  // 能量球符號
            } else if (grid[i][j] == '.') {
                gridDisplayColor[i][j] = "skin";  // 點數顏色
                gridDisplayThick[i][j] = "normal";  // 點數粗細
                gridDisplay[i][j] = ".";  // 點數符號
            } else if (grid[i][j] == ' ') {
                gridDisplayColor[i][j] = "white";  // 空白顏色
                gridDisplayThick[i][j] = "normal";  // 空白粗細
                gridDisplay[i][j] = " ";  // 空白符號
            } else if (isalpha(grid[i][j])) {
                if (grid[i][j] == 'G') {  // 嚇人的鬼
                    gridDisplayColor[i][j] = "white";
                    gridDisplayThick[i][j] = "normal";
                    gridDisplay[i][j] = "ö";  // 嚇人的鬼符號
                } else if (isupper(grid[i][j])) {
                    gridDisplayColor[i][j] = "purple";  // 鬼的顏色
                    gridDisplayThick[i][j] = "normal";  // 鬼的粗細
                    gridDisplay[i][j] = "ö";  // 鬼符號
                } else if (islower(grid[i][j])) {
                    string ghost[4] = {"red", "pink", "lightblue", "orange"};
                    gridDisplayColor[i][j] = ghost[grid[i][j] - 'a'];  // 小鬼的顏色
                    gridDisplayThick[i][j] = "normal";  // 小鬼的粗細
                    gridDisplay[i][j] = "∩";  // 小鬼符號
                }
            }
        }
    }

    cout << "\033[H";  // 移動光標到左上角
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (prevGrid[i][j] != grid[i][j]) {
                cout << "\033[" << (i + 1) << ";" << (j + 1) << "H";  // 移動光標到指定位置
                cout << outputColor(gridDisplayColor[i][j], "normal", gridDisplay[i][j]);  // 輸出顏色和符號
            }
        }
    }
    cout << "\033[" << (n + 1) << ";1H";  // 移動光標到下一行
    for (int i = 0; i < n; i++) {
        prevGrid[i] = grid[i];  // 更新前一個網格
    }
}

// 移動pacman
void movePacman(string grid[], int (&playerPos)[2], int &curDir, KeyManager &keyM) {
    int input = keyM.getLastKey();  // 獲取最後按下的鍵
    if (checkNotCollideWall(grid, playerPos[0] + dir[input][0],
                             playerPos[1] + dir[input][1])) {
        playerPos[0] += dir[input][0];  // 更新pacman位置
        playerPos[1] += dir[input][1];
        curDir = input;  // 更新當前方向
    } else {
        curDir = 4;  // 如果碰到牆壁，設置為停止
    }
    return;
}

// 重新生成鬼的位置
void respawnGhost(string grid[], int (&ghostPos)[2], int playerPos[2]) {
    int maxDistance = -1;
    int farX = -1, farY = -1;

    for (int x = 0; x < HEIGHT; x++) {
        for (int y = 0; y < WIDTH; y++) {
            if (grid[x][y] == '.' && (x != playerPos[0] || y != playerPos[1])) {
                int distance = (x - playerPos[0]) * (x - playerPos[0]) +
                               (y - playerPos[1]) * (y - playerPos[1]);
                if (distance > maxDistance) {
                    maxDistance = distance;
                    farX = x;
                    farY = y;  // 更新最遠位置
                }
            }
        }
    }
    ghostPos[0] = farX;  // 設置鬼的位置
    ghostPos[1] = farY;
}

// 更新遊戲狀態
void update(string grid[], int playerPos[2], int (*ghostPos)[2],
            int ghostDir[], char ghostType[], int ghostNum,
            string (&pointsGrid)[], int &pointsCnt, string (&display)[],
            int &score, bool &scaredMode, int &scaredTime) {
    int n = HEIGHT, m = WIDTH;
    for (int i = 0; i < n; i++) {
        string row = "";
        row.append(WIDTH, ' ');
        display[i] = row;  // 初始化顯示
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == '#') {
                display[i][j] = '#';  // 牆壁
            }
            if (pointsGrid[i][j] == '.') {
                display[i][j] = '.';  // 點數
            }
            if (pointsGrid[i][j] == '*') {
                display[i][j] = '*';  // 能量球
            }
        }
    }
    if (pointsGrid[playerPos[0]][playerPos[1]] == '.') {
        pointsGrid[playerPos[0]][playerPos[1]] = ' ';  // 吃掉點數
        score += 10;  // 正常得分
        pointsCnt--;  // 減少點數計數
    } else if (pointsGrid[playerPos[0]][playerPos[1]] == '*') {
        pointsGrid[playerPos[0]][playerPos[1]] = ' ';  // 吃掉能量球
        score += 50;  // 能量球得分
        scaredMode = true;  // 啟動驚嚇模式
        scaredTime = 20;  // 設置驚嚇時間為20，實際時間0.5*20為10秒
        // 將鬼的類型改為大寫
        for (int i = 0; i < ghostNum; i++) {
            ghostType[i] = toupper(ghostType[i]);
        }
    }
    // 減少驚嚇時間
    if (scaredMode) {
        scaredTime--;
        if (scaredTime <= 0) {
            scaredMode = false;  // 停止驚嚇模式
            // 將鬼的類型改回小寫
            for (int i = 0; i < ghostNum; i++) {
                ghostType[i] = tolower(ghostType[i]);
            }
        }
    }
    display[playerPos[0]][playerPos[1]] = '0';  // pacman位置
    for (int i = 0; i < ghostNum; i++) {
        // 如果剩下3秒，改變鬼的顏色
        if (scaredMode && scaredTime <= 6) {
            display[ghostPos[i][0]][ghostPos[i][1]] = 'G';  // 在驚嚇模式下用'G'表示鬼
        } else {
            display[ghostPos[i][0]][ghostPos[i][1]] = ghostType[i];  // 正常鬼顯示
        }
    }
}

#ifndef __VScode
BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT || signal == CTRL_CLOSE_EVENT) {
        gameRunning = false; // Set the game running flag to false
        return TRUE; // Indicate that the signal was handled
    }
    return FALSE; // Indicate that the signal was not handled
}
#endif
