#include "item.h"

// INIT
Coin::Coin(Game* g, LevelPos p) : Item(g, p, "coin.png") {

}

// CLEANUP
Coin::~Coin() {
    
}

// EVENTS
void Coin::collides() {
    Map* map = game->getMap();
    map->removeItem(this);
    map->getPlayer()->increaseCoins(5);
}

// UPDATE
void Coin::updateImpl() {
    
}
