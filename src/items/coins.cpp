#include "items.h"

// INIT
Coin::Coin(Game* g, H2DE_LevelPos p) : Item(g, p, "coin.png", 4), game(g) {

}

// CLEANUP
Coin::~Coin() {
    if (game->isDebuging()) std::cout << "Coin cleared" << std::endl;
}

// EVENTS
void Coin::collidedImpl() {
    
}

// UPDATE
void Coin::updateImpl() {

}
