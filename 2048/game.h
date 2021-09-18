#ifndef _GAME_H_
#define _GAME_H_

#include <Arduboy2.h>

#define AFTER_MOVE(pos, direction) pos + direction

const int emptyElement = 0;
const int firstElement = 0;

class Game {
   private:
    int score, fps, difficult;
    int height, width, size;
    int up, down, left, right;
    int *board, free, lastNewNumberIndex;
    bool lastMoveValid;

   public:
    Game(int width, int height, int difficult);
    ~Game();

    void moveOrMergeElement(int pos, int direction);
    bool checkBorderAfterMove(int pos, int direction);
    bool isSameLine(int pos1, int pos2);
    int rowAt(int pos);
    void operate(int direction);
    bool checkAlive();
    int getScore();
    int* getBoard();
    int getLastNewNumberIndex();
};

#endif