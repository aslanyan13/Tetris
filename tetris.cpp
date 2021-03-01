///////////////////////
//  Project: Tetris  //
//  Date:  1.3.2021  //
//  By: H. Aslanyan  //
///////////////////////

#include <iostream>
#include <time.h>
#include <math.h>
#include <cstdlib>

using namespace std;

const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 30;

const char tetriminos[7][4][5] = {
    // I Tetrimino
    {
        ".#..",
        ".#..",
        ".#..",
        ".#.."
    },
    // O Tetrimino
    {
        "....",
        ".##.",
        ".##.",
        "...."
    },
    // J Tetrimino
    {
        ".#..",
        ".#..",
        "##..",
        "...."
    },
    // L Tetrimino
    {
        ".#..",
        ".#..",
        ".##.",
        "...."
    },
    // T Tetrimino
    {
        "....",
        "###.",
        ".#..",
        "...."
    },
    // Z tetrimino
    {
        "##..",
        ".##.",
        "....",
        "...."
    },
    // S Tetrimino
    {
        ".##.",
        "##..",
        "....",
        "...."
    }
};

void tetriminoCopy (char tetrimino1[4][5], const char tetrimino2[4][5])
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 5; j++)
            tetrimino1[i][j] = tetrimino2[i][j];
}
void tetriminoDraw (int x, int y, char tetrimino[4][5], char map[MAP_HEIGHT][MAP_WIDTH])
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            if (map[y + i][x + j] == '.' && tetrimino[i][j] == '.') map[y + i][x + j]  = '.';
            else if (map[y + i][x + j] == '.' && tetrimino[i][j] == '#') map[y + i][x + j]  = '#';
        }
}

int main()
{
    srand(time(NULL)); // Randomizer seed seting

    char map[MAP_HEIGHT][MAP_WIDTH];
    char currentTetrimino[4][5];
    float tetrX = 10, tetrY = 2;
    uint8_t tetrNum = rand() % 7;

    do {
        system("clear");

        // Map generating
        for (int i = 0; i < MAP_HEIGHT; i++)
            for (int j = 0; j < MAP_WIDTH; j++)
                map[i][j] = '.';

        tetriminoCopy(currentTetrimino, tetriminos[tetrNum]);
        tetriminoDraw(tetrX, ceill(tetrY), currentTetrimino, map);

        // Map drawing
        for (int i = 0; i < MAP_HEIGHT; i++)
        {
            for (int j = 0; j < MAP_WIDTH; j++)
                cout << map[i][j];
            cout << endl;
        }

        tetrY += 0.01;

    } while (true);
    return 0;
}


