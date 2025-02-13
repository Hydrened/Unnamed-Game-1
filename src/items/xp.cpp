#include "items.h"

// INIT
Xp::Xp(Game* g, H2DE_LevelPos p, int l) : Item(g, p, "xp-lvl-" + std::to_string(l) + ".png", 2), level(l) {

}

// CLEANUP
Xp::~Xp() {
    std::cout << "Xp cleared" << std::endl;
}

// EVENTS
void Xp::collidedImpl() {
    
}

// UPDATE
void Xp::updateImpl() {

}
