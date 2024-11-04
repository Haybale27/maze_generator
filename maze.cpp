#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>   // For std::shuffle
#include <random>      // For std::default_random_engine
#include <fstream>     // For file output
#include <thread>      // For std::this_thread::sleep_for
#include <chrono>      // For std::chrono::milliseconds
#include <windows.h>
#include <conio.h>
#include <stack>

using namespace std;

const int WIDTH = 33;  // Must be odd for proper maze generation
const int HEIGHT = 33; // Must be odd for proper maze generation
const int OUTPUTX = 3;
const int OUTPUTY = 1;
const int displaySpeedMilliseconds = 0;
const int solveDisplaySpeed = 0;
string wallStr(OUTPUTX, '#');
string emptyStr(OUTPUTX, ' ');
string playerStr = " P ";
string visitedStr = " . ";

vector<vector<bool>> maze(HEIGHT, vector<bool>(WIDTH, false));
vector<vector<bool>> visitedCells(HEIGHT, vector<bool>(WIDTH, true));
//vector<pair<int, int>> visitedCells;

pair<int, int> startingPos = {1, HEIGHT - 2};
pair<int, int> endingPos = {WIDTH - 2, 1};
pair<int, int> currentPos = startingPos;
stack<pair<int, int>> solvePath;

void gotoxy(int x, int y);
void initializeMaze();
void displayMaze();
void updateDisplayMaze(int x, int y);
bool isInBounds(int x, int y);
void walk(int x, int y);
bool hunt();
void generateMaze();

void solveMaze();
void displayCurrentPos(int x, int y);
void displayVisitedCell(int x, int y);
void displaySolvePath();
pair<int, int> checkWall(int x, int y);
void backTrack();
bool checkVisited(int x, int y);
pair<int, int> check(int x, int y);

int main() {
    generateMaze();
    // Write maze to file
    ofstream outFile("maze_output.txt");
    if (outFile.is_open()) {
        for (int y = 0; y < HEIGHT; y++) {
            for (int i = y; i < y + OUTPUTY; i++){
                for (int x = 0; x < WIDTH; x++) {
                    outFile << (maze[x][y] ? emptyStr : wallStr);
                }
                outFile << "\n";
            }
        }
        outFile.close();
        cout << "Maze output saved to maze_output.txt\n";
    } else {
        cerr << "Error opening file!\n";
    }
    displayMaze();
    solveMaze();
    displayCurrentPos(currentPos.first, currentPos.second);
    gotoxy(0, HEIGHT * OUTPUTY);

    return 0;
}


void gotoxy(int x, int y)
{
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void initializeMaze() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            maze[y][x] = false;  // Initialize all cells as walls
            visitedCells[y][x] = true;
        }  
    }
}

void displayMaze() {
    system("cls");
    gotoxy(0, 0);
    for (int y = 0; y < HEIGHT; y++) {
        for (int i = y; i < y + OUTPUTY; i++){
            for (int x = 0; x < WIDTH; x++) {
                cout << (maze[x][y] ? emptyStr : wallStr);
            }
            cout << "\n";
        }
    }
}
void updateDisplayMaze(int x, int y) {
    for (int i = y * OUTPUTY; i < (y * OUTPUTY) + OUTPUTY; i++) {
        gotoxy(x * OUTPUTX, i);
        printf("%s", emptyStr.c_str(), x * OUTPUTX, i);
    }
}

bool isInBounds(int x, int y) {
    return x >= 1 && y >= 1 && x < WIDTH - 1 && y < HEIGHT - 1;
}

void walk(int x, int y) {
    maze[y][x] = true;
    visitedCells[y][x] = false;
    updateDisplayMaze(x, y);  // Display the maze after updating the current position
    this_thread::sleep_for(chrono::milliseconds(displaySpeedMilliseconds)); // Delay
    vector<pair<int, int>> directions = {{0, 2}, {2, 0}, {0, -2}, {-2, 0}};
    random_device rd;
    default_random_engine rng(rd());
    shuffle(directions.begin(), directions.end(), rng);

    for (const auto& [dx, dy] : directions) {
        int nx = x + dx, ny = y + dy;
        if (isInBounds(nx, ny) && !maze[ny][nx]) {
            maze[x + dx / 2][y + dy / 2] = true; // Open wall between cells
            visitedCells[x + dx / 2][y + dy / 2] = false;
            updateDisplayMaze(x + dx / 2, y + dy / 2);  // Display the maze after updating the current position
            this_thread::sleep_for(chrono::milliseconds(displaySpeedMilliseconds)); // Delay
            walk(nx, ny);  // Continue walking
        }
    }
}

bool hunt() {
    for (int y = 1; y < HEIGHT; y += 2) {
        for (int x = 1; x < WIDTH; x += 2) {
            if (!maze[y][x]) {
                vector<pair<int, int>> neighbors = {{0, 2}, {2, 0}, {0, -2}, {-2, 0}};
                for (const auto& [dx, dy] : neighbors) {
                    int nx = x + dx, ny = y + dy;
                    if (isInBounds(nx, ny) && maze[ny][nx]) {
                        maze[y][x] = maze[y + dy / 2][x + dx / 2] = true;
                        visitedCells[y][x] = visitedCells[y + dy / 2][x + dx / 2] = false;
                        walk(x, y);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void generateMaze() {
    initializeMaze();
    displayMaze();
    walk(1, 1);
    while (hunt());
}

void solveMaze() {
    while (currentPos != endingPos){
        displayCurrentPos(currentPos.first, currentPos.second);
        this_thread::sleep_for(chrono::milliseconds(solveDisplaySpeed));
        solvePath.push(currentPos);
        visitedCells[currentPos.first][currentPos.second] = true;
        pair<int, int> nextCell = check(currentPos.first, currentPos.second);
        if (nextCell.first == -1){
            backTrack();
        } else {
            displayVisitedCell(currentPos.first, currentPos.second);
            currentPos = nextCell;
        }
    }
}

void backTrack(){
    do {
        //displayVisitedCell(currentPos.first, currentPos.second);
        updateDisplayMaze(currentPos.first, currentPos.second);
        solvePath.pop();
        currentPos = solvePath.top();
        displayCurrentPos(currentPos.first, currentPos.second);
        this_thread::sleep_for(chrono::milliseconds(solveDisplaySpeed));
    } while (check(currentPos.first, currentPos.second).first == -1);
    displayVisitedCell(currentPos.first, currentPos.second);
    currentPos = check(currentPos.first, currentPos.second);
}

pair<int, int> check(int x, int y){
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    for (const auto& [dx, dy] : directions) {
        int nx = x + dx, ny = y + dy;
        if (!visitedCells[nx][ny]){
            return make_pair(nx, ny);
        }
    }
    return {-1, -1};
}

void displayCurrentPos(int x, int y) {
    for (int i = y * OUTPUTY; i < (y * OUTPUTY) + OUTPUTY; i++) {
        gotoxy(x * OUTPUTX, i);
        printf("%s", playerStr.c_str(), x * OUTPUTX, i);
    }
}
void displayVisitedCell(int x, int y){
    for (int i = y * OUTPUTY; i < (y * OUTPUTY) + OUTPUTY; i++) {
        gotoxy(x * OUTPUTX, i);
        printf("%s", visitedStr.c_str(), x * OUTPUTX, i);
    }
}
void displaySolvePath(){

}












