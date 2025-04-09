#include <bits/stdc++.h>
#include <windows.h>
//#define __VScode //若使用VScode請取消註解
#include "pacman.hpp"
using namespace std;

int dir[5][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {0, 0}};  // d,s,a,w,stop
int HEIGHT = 15, WIDTH = 66;  // 定義遊戲區域的高度和寬度
bool gameRunning = true;

// 檢查是否與牆壁碰撞
bool checkNotCollideWall(string grid[], int i, int j) {
    int n = HEIGHT, m = WIDTH;
    if (/*question 1*/)
        return false;  // 如果超出邊界或碰到牆壁，回傳false
    return true;  // 否則回傳true
}

// 檢查是否與鬼碰撞
bool checkCollideGhost(int (*ghostPos)[2], int playerPos[2], int ghostNum) {
    for (int i = 0; i < ghostNum; i++) {
        if (/*question 2*/) {
            return true;  // 如果pacman與鬼的座標相同，回傳true
        }
    }
    return false;  // 否則回傳false
}

// 計算最佳方向
int bestDir(string grid[], int curPos[2], int curDir, int target[2]) {
    int n = HEIGHT, m = WIDTH;

    int ans = 4;  // 預設為停止
    int minDis = INT16_MAX;  // 最小距離初始化為最大值
    for (int i = 0; i < 4; i++) {
        if (i == (curDir + 2) % 4)
            continue;  // 不考慮反方向

        int newI = curPos[0] + dir[i][0]; //新的i座標
        int newJ = curPos[1] + dir[i][1]; //新的j座標
        int dis = /*question 3*/ ;
        if (dis < minDis && checkNotCollideWall(grid, newI, newJ)) {
            ans = i;  // 更新最佳方向
            minDis = dis;  // 更新最小距離
        }
    }
    return ans;  // 回傳最佳方向
}


// 移動鬼
template <int _N>
void moveGhost(string grid[], int (&ghostPos)[_N][2], int (&ghostDir)[_N],
                char ghostType[], int ghostNum, int playerPos[2],
                int playerDir, bool scaredMode) {
    for (int i = 0; i < ghostNum; i++) {
        int legalRoute = 0;  // 合法路徑計數
        int possibleDir[3];  // 存儲可能的方向
        for (int j = 0; j < 4; j++) {
            if ((checkNotCollideWall(grid, ghostPos[i][0] + dir[j][0],
                                      ghostPos[i][1] + dir[j][1])) &&
                (j != (ghostDir[i] + 2) % 4)) {
                possibleDir[legalRoute] = j;  // 存儲有效方向
                legalRoute++;
            }
        }
        legalRoute++;

        if (!scaredMode) {  // 如果不是驚嚇模式
            if (legalRoute == 1) {
                ghostDir[i] = (ghostDir[i] + 2) % 4;  // 反向移動
            } else if (legalRoute == 2) {
                ghostDir[i] = possibleDir[0];  // 隨機選擇一個方向
            } else if (legalRoute == 3 || legalRoute == 4) {
                int target[2] = {};
                if (ghostType[i] == 'a') {  // 紅鬼：目標pacman位置
                    /*question 4*/
                } else if (ghostType[i] == 'b') {  // 粉紅鬼：目標pacman位置4格前
                    /*question 5*/
                } else if (ghostType[i] == 'c') {  // 淺藍鬼：目標與紅色鬼對稱中心在pacman前方2格
                    int indexOfRed = 0;
                    for (int j = 0; j < ghostNum; j++) {
                        if (ghostType[j] == 'a') {
                            indexOfRed = j;
                            break;
                        }
                    }
                    int center[2];
                    center[0] = playerPos[0] + dir[playerDir][0] * 2;
                    center[1] = playerPos[1] + dir[playerDir][1] * 2;
                    /*question 6*/
                } else if (ghostType[i] == 'd') {  // 橙鬼：保持4格距離
                    double a, b;
                    a = ghostPos[i][0] - playerPos[0];
                    b = ghostPos[i][1] - playerPos[1];
                    target[0] = round(playerPos[0] + a * 4 / sqrt(a * a + b * b));
                    target[1] = round(playerPos[1] + b * 4 / sqrt(a * a + b * b));
                }
                ghostDir[i] = bestDir(grid, ghostPos[i], ghostDir[i], target);  // 計算最佳方向
            }
        } else {  // 如果是驚嚇模式
            if (isupper(ghostType[i])) {
                if (legalRoute == 1) {
                    ghostDir[i] = (ghostDir[i] + 2) % 4;  // 反向移動
                } else if (legalRoute == 2) {  // 不回頭
                    ghostDir[i] = possibleDir[0];
                } else if (legalRoute == 3 || legalRoute == 4) {  // 不回頭
                    ghostDir[i] = possibleDir[rand() % (legalRoute - 1)];
                }
            }
            if (islower(ghostType[i])) {  // 復活先不動
                ghostDir[i] = 4;
            }
        }
        if (checkNotCollideWall(grid, ghostPos[i][0] + dir[ghostDir[i]][0],
                                 ghostPos[i][1] + dir[ghostDir[i]][1])) {
            ghostPos[i][0] += dir[ghostDir[i]][0];  // 根據方向移動
            ghostPos[i][1] += dir[ghostDir[i]][1];
        }
    }
}

// 吃掉鬼
void eatGhost(string grid[], int playerPos[2], int (*ghostPos)[2],
               int (&ghostDir)[], char (&ghostType)[], int ghostNum,
               int &score) {
    for (int i = 0; i < ghostNum; i++) {
        if (ghostPos[i][0] == playerPos[0] && ghostPos[i][1] == playerPos[1]) {
            score += 200;  // 吃掉鬼的得分
            /*question 7*/
            ghostDir[i] = 4;  // 設置鬼的方向為停止
            ghostType[i] = tolower(ghostType[i]);  // 將鬼的類型改為小寫
        }
    }
}

int main() {
    srand(time(0));  // 設置隨機數種子
    
#ifndef __VScode
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);
    system("chcp 65001");
#endif
    string initGrid[HEIGHT] = {
        "#################################################################",
        "#...............................................................#",
        "#..###...#...##...##.....#####....#......#...#######...#######..#",
        "#..#.#...#....##.##.....##...##...#......#...#.........#........#",
        "#..#.###.#.....###......#.........#......#...#######...#######..#",
        "#..#...#.#......#.......##...##...##....##...#.........#........#",
        "#..#...###......#........#####.....######....#######...#######..#",
        "#...............................0...............................#",
        "#...#####.....#####....##....##...########...#######...#######..#",
        "#..##...##...##   ##...###..###...#      #.........#...#........#",
        "#..#.........#######...#.####.#...########...#######...#######..#",
        "#..##...##...#.....#...#..##..#...#..........#...............#..#",
        "#...#####....#.....#...#......#...#..........#######...#######..#",
        "#...............................................................#",
        "#################################################################"};
    const int powerNum = 4, ghostNum = 4;  // 能量球和鬼的數量
    int pointsCnt = 0;

    int playerPos[2];  // pacman位置
    int playerDir = 3;  // 初始方向為'w'

    int ghostPos[ghostNum][2];  // 鬼的位置
    int ghostDir[ghostNum] = {4, 4, 4, 4};  // 鬼的方向
    char ghostType[ghostNum];  // 鬼的類型

    int score = 0;  // 分數
    int scaredTime = 0;  // 驚嚇時間
    bool scaredMode = false;  // 是否在驚嚇模式

    string pointsGrid[HEIGHT];  // 點數網格
    string gridDisplay[HEIGHT];  // 顯示網格
    string prevGridDisplay[HEIGHT];  // 前一個顯示網格

    for (int i = 0; i < HEIGHT; i++) {
        string row = "";
        row.append(WIDTH, ' ');
        pointsGrid[i] = row;  // 初始化點數網格
        gridDisplay[i] = row;  // 初始化顯示網格
        prevGridDisplay[i] = row;  // 初始化前一個顯示網格
    }

    for (int i = 0; i < HEIGHT; i++) {  // 設定pacman位置的初始狀態
        for (int j = 0; j < WIDTH; j++) {
            char c = initGrid[i][j];
            if (c == '0') {
                /*question 8*/
            }
        }
    }

    // 隨機放置能量球
    for (int p = 0; p < powerNum; p++) {
        int x, y;
        do {
            x = rand() % HEIGHT;  // 隨機列
            y = rand() % WIDTH;  // 隨機行
        } while (/*question 9*/);
        initGrid[x][y] = '*';  // 放置能量球
    }

    // 隨機放置鬼
    for (int i = 0; i < ghostNum; i++) {
        int x, y;
        do {
            x = rand() % HEIGHT;  // 隨機列
            y = rand() % WIDTH;  // 隨機行
        } while (!(initGrid[x][y] == '.' &&
                    ((x - playerPos[0]) * (x - playerPos[0]) +
                     (y - playerPos[1]) * (y - playerPos[1])) >= 49));
        // 確保放置在合法位置且距離pacman有一定距離
        ghostPos[i][0] = x;  // 設置鬼的行
        ghostPos[i][1] = y;  // 設置鬼的列
        ghostType[i] = 'a' + i;  // 分配鬼的類型 a, b, c, d
    }

    for (/*question 10-1*/) {  // 設定點數網格的初始狀態
        for (/*question 10-2*/) {
            char c = initGrid[i][j];
            gridDisplay[i][j] = c;  // 初始化顯示網格
            if (c == '.' || c == '*') {
                pointsGrid[i][j] = c;  // 設置點數網格
                pointsCnt++;  // 增加點數計數
            }
        }
    }

    KeyManager keyM;  // 創建鍵盤管理器
    int curDir = playerDir;  // 當前方向
    bool gameOver = false;  // 遊戲是否結束

    cout << "\x1B[2J\x1B[H" << "\033[?25l";  // 初始化螢幕，清除並隱藏鼠標
    update(initGrid, playerPos, ghostPos, ghostDir, ghostType, ghostNum, pointsGrid, pointsCnt, gridDisplay, score, scaredMode, scaredTime);
    gridPrint(gridDisplay, prevGridDisplay);
    cout << "\033[" << (HEIGHT + 1) << ";1H\033[K";
    cout << "press any key to start game (use arrows or WASD to move pacman)" << endl;
    keyM.anyKeyToContinue();
    for (int i = 3; i > 0; i--) {
        cout << "\033[" << (HEIGHT + 1) << ";1H\033[K";
        cout << "starting in " << i << "........." << endl;
        Sleep(1000);
    }
   
    while (!gameOver && gameRunning) {
        // 移動pacman
        movePacman(initGrid, playerPos, playerDir, keyM);  
        // 檢查是否與鬼碰撞
        if (checkCollideGhost(ghostPos, playerPos, ghostNum)) {  
            if (scaredMode) 
                eatGhost(initGrid, playerPos, ghostPos, ghostDir, ghostType, ghostNum, score);  // 吃掉鬼
            else 
                gameOver = true;  // 如果不是驚嚇模式，遊戲結束
        }
        // 移動鬼
        if (!(scaredMode && scaredTime % 2)) // 如果是驚嚇模式只有一半的時間會移動鬼
            moveGhost(initGrid, ghostPos, ghostDir, ghostType, ghostNum, playerPos, playerDir, scaredMode);  
        // 再次檢查是否與鬼碰撞
        if (checkCollideGhost(ghostPos, playerPos, ghostNum)) {  
            if (scaredMode) 
                eatGhost(initGrid, playerPos, ghostPos, ghostDir, ghostType, ghostNum, score);  // 吃掉鬼
            else 
                gameOver = true;  // 如果不是驚嚇模式，遊戲結束
        }
        // 更新遊戲狀態
        update(initGrid, playerPos, ghostPos, ghostDir, ghostType, ghostNum, pointsGrid, pointsCnt, gridDisplay, score, scaredMode, scaredTime);  
        // 輸出遊戲網格
        gridPrint(gridDisplay, prevGridDisplay);  

        cout << "\033[" << (HEIGHT + 1) << ";1H\033[K";  // 移動鼠標到顯示分數的位置
        cout << "Score: " << score << endl;  // 顯示分數
        Sleep(500);  // 暫停0.5秒
        if (pointsCnt == 0) gameOver = true;  // 如果所有點數都被吃掉，遊戲結束
    }

    //遊戲結束
    cout << "\033[?25h" << "\033[" << (HEIGHT + 2) << ";1H";  // 恢復鼠標顯示

    if (pointsCnt == 0) {
        cout << "You win! Final score: " << score << endl;  // 如果贏了，顯示勝利信息
    } else {
        cout << "Game Over! Final score: " << score << endl;  // 如果遊戲結束，顯示結束信息
    }
    return 0;  // 回傳0，結束程序
}
