#ifndef SAVE_H
#define SAVE_H

#include "game.h"
class Game;

class Save {
private:
    Game* game;

    int coins;

public:
    Save(Game* game);
    ~Save() noexcept(false);

    int getCoins() const;

    void setCoins(int coins);
};

#endif
