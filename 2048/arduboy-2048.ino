#include "game.h"

#define UP -4
#define DOWN 4
#define LEFT -1
#define RIGHT 1

#define GRID_HEIGHT 15
#define GRID_WIDTH 20

// Make an instance of arduboy used for many functions
Arduboy2 arduboy;
Game* game;
int highScore;

// This function runs once in your game.
// use it for anything that needs to be set only once in your game.
void setup() {
    arduboy.begin();  // initiate arduboy instance
    arduboy.setFrameRate(30);
    arduboy.initRandomSeed();

    game = new Game(4, 4, 2);
}

void render() {
    int* board = game->getBoard();
    int newPos = game->getLastNewNumberIndex();
    int score = game->getScore();

    for (int n = 0; n <= 4 * GRID_HEIGHT; n += GRID_HEIGHT) {  //绘制5条横线
        arduboy.drawLine(0, n, 4 * GRID_WIDTH, n, 1);
        for (int m = 0; m <= 4 * GRID_WIDTH; m += GRID_WIDTH)  //绘制5条竖线
            arduboy.drawLine(m, 0, m, 4 * GRID_HEIGHT, 1);
    }

    for (int x = 0; x < 4; x++) {  //输出数组内数据
        for (int y = 0; y < 4; y++) {
            arduboy.setCursor(2 + (y * GRID_WIDTH), 4 + (x * GRID_HEIGHT));
            if (board[x * 4 + y] != 0)
                arduboy.print(board[x * 4 + y]);
        }
    }

    arduboy.setCursor(4 * GRID_WIDTH + 4, 0);
    arduboy.print("score:");

    arduboy.setCursor(4 * GRID_WIDTH + 6, 10);
    arduboy.print(score);

    arduboy.setCursor(4 * GRID_WIDTH + 4, 20);
    arduboy.print("highest:");

    arduboy.setCursor(4 * GRID_WIDTH + 6, 30);
    arduboy.print(highScore);
    // arduboy.print(newPos);
}

void loop() {
    // pause render until it's time for the next frame
    if (!(arduboy.nextFrame()))
        return;

    arduboy.pollButtons();

    if (arduboy.justPressed(UP_BUTTON))
        game->operate(UP);
    if (arduboy.justPressed(DOWN_BUTTON))
        game->operate(DOWN);
    if (arduboy.justPressed(LEFT_BUTTON))
        game->operate(LEFT);
    if (arduboy.justPressed(RIGHT_BUTTON))
        game->operate(RIGHT);

    if (!game->checkAlive()) {
        highScore = game->getScore();
        game = new Game(4, 4, 2);
    } else {
        arduboy.clear();  // clear our screen to black
        render();
        arduboy.display();  // display.
    }
}