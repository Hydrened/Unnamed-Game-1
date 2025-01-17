#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "game.h"
class Game;

class Calculator {
private:
    Game* game;

    int blockSize;
    int winWidth;
    int winHeight;

public:
    Calculator(Game* game);
    ~Calculator();

    H2DE_Pos convertToPx(LevelPos pos);
    H2DE_Size convertToPx(LevelSize size);
    LevelPos computePxPos(int x, int y);
};

#endif
