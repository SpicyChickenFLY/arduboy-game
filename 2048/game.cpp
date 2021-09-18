#include "game.h"

Game::Game(int width, int height, int difficult) {
    fps = 60;
    score = 0;
    lastMoveValid = false;
    this->difficult = difficult;
    this->width = width;
    this->height = height;
    size = width * height;
    up = -1 * width;
    down = width;
    left = -1;
    right = 1;
    board = new int[size]{0};

    int pos1 = random(size);
    int pos2 = random(size - 1);
    if (pos2 >= pos1)
        pos2++;
    board[pos1] = random(difficult) + 1;
    board[pos2] = random(difficult) + 1;
    free = size - 2;
}

Game::~Game() {
    delete board;
}

bool Game::checkBorderAfterMove(int pos, int direction) {
    int newPos = AFTER_MOVE(pos, direction);
    if (newPos < firstElement || newPos >= size)
        return false;  // out of vertical border
    if ((direction == left || direction == right) && !isSameLine(pos, newPos))
        return false;  // out of horizon border
    return true;
}

bool Game::isSameLine(int pos1, int pos2) {
    return rowAt(pos1) == rowAt(pos2);
}

int Game::rowAt(int pos) {
    return pos / width;
}

void Game::moveOrMergeElement(int pos, int direction) {
    int newPos = AFTER_MOVE(pos, direction);

    if (board[pos] == emptyElement || !checkBorderAfterMove(pos, direction)) {
        return;  // can not be moved, end for next element
    }

    if (board[newPos] == board[pos]) {  // can be merged
        score += board[pos];
        board[newPos]++;      // promote the target element
        board[newPos] *= -1;  // mark the target not be merged again
        board[pos] = emptyElement;
        free++;
        lastMoveValid = true;
    }

    if (board[newPos] == emptyElement) {  // can be moved
        board[newPos] = board[pos];       // assign the target element
        board[pos] = emptyElement;
        moveOrMergeElement(newPos, direction);  // (iteration)
        lastMoveValid = true;
    }
}

void Game::operate(int direction) {
    lastMoveValid = false;
    // first loop: move or merge
    if (direction == up || direction == left) {
        for (int pos = firstElement; pos < size; pos++) {
            moveOrMergeElement(pos, direction);
        }
    } else {
        for (int pos = size - 1; pos >= firstElement; pos--) {
            moveOrMergeElement(pos, direction);
        }
    }

    // second loop: clean marks, judge alive, generate new number
    int newNumCountDown = -1;
    if (lastMoveValid) {  // choose a free space randomly for new number
        newNumCountDown = random(free);
    }
    for (int pos = firstElement; pos < size; pos++) {
        if (board[pos] < 0) {  // clean this mark by making it positive
            board[pos] = -board[pos];
        } else if (board[pos] == emptyElement) {
            if (newNumCountDown == 0) {  // assign this space for new number
                board[pos] = random(difficult) + 1;
                free--;  // free space decreased because of the new number
                lastNewNumberIndex = pos;
            }
            if (newNumCountDown >= 0)  // wait for next free pos
                newNumCountDown--;
        }
    }
}

bool Game::checkAlive() {
    if (free > 0) {
        // if not valid (never move or merge this turn), end for next operation
        return true;
    } else {
        // we ignore the last element
        for (int pos = 0; pos < size - 1; pos++) {
            // ignore last row while chechk two row && ignore last column while
            // check two column
            if (pos < size - width &&
                    board[pos] == board[AFTER_MOVE(pos, down)] ||
                pos % width < width - 1 &&
                    board[pos] == board[AFTER_MOVE(pos, right)]) {
                return true;
            }
        }
        return false;
    }
}

int Game::getScore() {
    return score;
}

int* Game::getBoard() {
    return board;
}

int Game::getLastNewNumberIndex() {
    return lastNewNumberIndex;
}