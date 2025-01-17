#include "calculator.h"

// INIT
Calculator::Calculator(Game* g) : game(g) {
    winWidth = game->getData()->sizes->winWidth;
    winHeight = game->getData()->sizes->winHeight;
    blockSize = game->getData()->sizes->blockSize;
}

// CLEANUP
Calculator::~Calculator() {

}

// CONVERT
H2DE_Pos Calculator::convertToPx(LevelPos pos) {
    static Camera* camera = game->getCamera();
    LevelPos camPos = camera->getPos();

    return {
        static_cast<int>((pos.x - camPos.x) * blockSize),
        static_cast<int>((pos.y - camPos.y) * blockSize)
    };
}

H2DE_Size Calculator::convertToPx(LevelSize size) {
    return {
        static_cast<int>(size.w * blockSize),
        static_cast<int>(size.h * blockSize)
    };
}

// COPMUTE
LevelPos Calculator::computePxPos(int x, int y) {
    static int blockSize = game->getData()->sizes->blockSize;
    LevelPos c = game->getCamera()->getPos();

    return {
        (float)x / blockSize + c.x,
        (float)y / blockSize + c.y
    };
}
