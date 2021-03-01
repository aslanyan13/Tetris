///////////////////////
//  Project: Tetris  //
//  Date:  1.3.2021  //
//  By: H. Aslanyan  //
///////////////////////

#include <iostream>
#include <time.h>
#include <math.h>
#include <cstdlib>
#include <map>
#include <chrono>
#include <vector>

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 30;

const int BLOCK_WIDTH = 15;
const int BLOCK_HEIGHT = 15;

char currentSym = '#';

const char tetrominos[7][4][5] = {
    // I Tetromino
    {
        ".#..",
        ".#..",
        ".#..",
        ".#.."
    },
    // O Tetromino
    {
        "....",
        ".##.",
        ".##.",
        "...."
    },
    // J Tetromino
    {
        ".#..",
        ".#..",
        "##..",
        "...."
    },
    // L Tetromino
    {
        ".#..",
        ".#..",
        ".##.",
        "...."
    },
    // T Tetromino
    {
        "....",
        "###.",
        ".#..",
        "...."
    },
    // Z tetromino
    {
        "##..",
        ".##.",
        "....",
        "...."
    },
    // S Tetromino
    {
        ".##.",
        "##..",
        "....",
        "...."
    }
};

// Figure copying
void tetrominoCopy (char tetromino1[4][5], const char tetromino2[4][5])
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 5; j++)
        {
            if (tetromino2[i][j] == '#')
                tetromino1[i][j] = currentSym;
            else
                tetromino1[i][j] = tetromino2[i][j];
        }
}
// Figure drawing in map
void tetrominoDraw (int x, int y, char tetromino[4][5], char map[MAP_HEIGHT][MAP_WIDTH])
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            if (map[y + i][x + j] == '.' && tetromino[i][j] == '.') map[y + i][x + j]  = '.';
            else if (map[y + i][x + j] == '.' && tetromino[i][j] == currentSym) map[y + i][x + j]  = currentSym;
        }
}
// Line checking
void checkLines (char map[MAP_HEIGHT][MAP_WIDTH])
{
    vector <int> fullLines;

    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        bool isFull = true;
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            if (map[i][j] == '.') isFull = false;
        }
        if (isFull) fullLines.push_back(i);
    }

    for (int i = 0; i < fullLines.size(); i++)
    {
        for (int j = fullLines[i]; j > 0; j--) {
            for (int k = 0; k < MAP_WIDTH; k++)
                map[j][k] = map[j - 1][k];
        }
    }

}

// Checking if figure can fitt
bool isFitt (int x, int y, char tetromino[4][5], char map[MAP_HEIGHT][MAP_WIDTH]) {

    int leftEdge = -1, rightEdge = -1, bottomEdge = -1;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (tetromino[j][i] == currentSym) leftEdge = i;
        }
        if (leftEdge != -1) break;
    }
    for (int i = 3; i >= 0; i--)
    {
        for (int j = 0; j < 4; j++)
        {
            if (tetromino[j][i] == currentSym) rightEdge = i;
        }
        if (rightEdge != -1) break;
    }
    for (int i = 3; i >= 0; i--)
    {
        for (int j = 0; j < 4; j++)
        {
            if (tetromino[i][j] == currentSym) bottomEdge = i;
        }
        if (bottomEdge != -1) break;
    }

    if (x + leftEdge < 0) return false;
    if (x + rightEdge > MAP_WIDTH - 1) return false;
    if (y + bottomEdge > MAP_HEIGHT - 1) return false;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (map[y + i][x + j] != '.' && tetromino[i][j] == currentSym) return false;
        }
    }

    return true;
}

// Figure rotating
void tetrominoRotate (char tetromino[4][5], int tetrNum) {
    int rotationPlaceN = 3;

    if (tetrNum == 0 || tetrNum == 1) {
        rotationPlaceN = 4;
    }

    //transpose
    for(int r = 0; r < rotationPlaceN; r++) {
      for(int c = r; c < rotationPlaceN; c++) {
        swap(tetromino[r][c], tetromino[c][r]);
      }
    }

    //reverse elements on row order
    for(int r = 0; r < rotationPlaceN; r++) {
        for(int c =0; c < rotationPlaceN/2; c++) {
          swap(tetromino[r][c], tetromino[r][rotationPlaceN-c-1]);
        }
    }
}

int main()
{
    srand(time(NULL)); // Randomizer seed seting
    const string symbols = "#@$%&^";

    map <char, Color> colors;
    colors['#'] = Color::Red;
    colors['@'] = Color::Blue;
    colors['$'] = Color::Green;
    colors['%'] = Color::Magenta;
    colors['^'] = Color::Yellow;
    colors['&'] = Color::Cyan;

    RenderWindow window (VideoMode(MAP_WIDTH * BLOCK_WIDTH, MAP_HEIGHT * BLOCK_WIDTH), "Tetris");

    char map[MAP_HEIGHT][MAP_WIDTH];
    char currentTetromino[4][5];
    float tetrX = 10, tetrY = 0;
    int tetrNum = rand() % 7;

    // Map generating
    for (int i = 0; i < MAP_HEIGHT; i++)
        for (int j = 0; j < MAP_WIDTH; j++)
            map[i][j] = '.';

    tetrominoCopy(currentTetromino, tetrominos[tetrNum]);

    unsigned int timer = 0;
    while (window.isOpen())
    {
        window.clear();

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        // tetrominoClear(tetrX, ceill(tetrY), currentTetromino, map);

        char temp[4][5];
        tetrominoCopy(temp, currentTetromino);

        tetrominoRotate(temp, tetrNum);

        if (fmod(timer, 2) == 0)
        {
            if (isFitt(tetrX, ceill(tetrY), temp, map) && Keyboard::isKeyPressed(Keyboard::Space))
                tetrominoRotate(currentTetromino, tetrNum);
            if (isFitt(tetrX - 1, ceill(tetrY), currentTetromino, map) && Keyboard::isKeyPressed(Keyboard::Left))
                tetrX--;
            if (isFitt(tetrX + 1, ceill(tetrY), currentTetromino, map) && Keyboard::isKeyPressed(Keyboard::Right))
                tetrX++;
            if (isFitt(tetrX, ceill(tetrY + 0.2), currentTetromino, map) && Keyboard::isKeyPressed(Keyboard::Down))
                tetrY += 0.2;
        }

        system("clear");

        for (int i = 0; i < MAP_HEIGHT; i++)
        {
            for (int j = 0; j < MAP_WIDTH; j++)
            {
                cout << map[i][j];
            }
            cout << endl;
        }

        // Map drawing
        for (int i = 0; i < MAP_HEIGHT; i++)
        {
            for (int j = 0; j < MAP_WIDTH; j++)
            {
                RectangleShape block;
                block.setSize(Vector2f(BLOCK_WIDTH - 2, BLOCK_HEIGHT - 2));
                block.setOutlineThickness(1);
                block.setOutlineColor(Color::Black);
                block.setPosition(j * BLOCK_WIDTH, i * BLOCK_HEIGHT);
                block.setFillColor(colors[map[i][j]]);

                if (map[i][j] != '.') window.draw(block);
            }
        }

        // Current tetromino figure drawing
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                RectangleShape block;
                block.setSize(Vector2f(BLOCK_WIDTH - 2, BLOCK_HEIGHT - 2));
                block.setOutlineThickness(1);
                block.setOutlineColor(Color::Black);
                block.setPosition(tetrX * BLOCK_WIDTH + j * BLOCK_WIDTH, (tetrY * BLOCK_HEIGHT) + i * BLOCK_HEIGHT);
                block.setFillColor(colors[currentSym]);

                if (currentTetromino[i][j] != '.') window.draw(block);
            }
        }

        cout << tetrY << endl;

        if (isFitt(tetrX, ceill(tetrY + 0.1), currentTetromino, map)) {
            tetrY += 0.1;
        }
        else {
            // New figure creating
            tetrominoDraw(tetrX, ceill(tetrY), currentTetromino, map);
            tetrX = 10, tetrY = 0;
            tetrNum = rand() % 7;
            currentSym = symbols[rand() % symbols.length()];
            tetrominoCopy(currentTetromino, tetrominos[tetrNum]);

            // Game over test
            if (!isFitt(tetrX, tetrY, currentTetromino, map)) break;
        }

        window.display();

        checkLines(map); // Lines checking
        timer++;         // Game timer
    }

    return 0;
}