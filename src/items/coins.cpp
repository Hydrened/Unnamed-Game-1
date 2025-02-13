#include "items.h"

// INIT
Coin::Coin(Game* g, H2DE_LevelPos p) : Item(g, p, "coin.png", 4) {

}

// CLEANUP
Coin::~Coin() {
    std::cout << "Coin cleared" << std::endl;
}

// EVENTS
void Coin::collidedImpl() {
    
}

// UPDATE
void Coin::updateImpl() {

}
