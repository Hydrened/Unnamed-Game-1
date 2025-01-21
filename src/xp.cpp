#include "item.h"

// INIT
Xp::Xp(Game* g, LevelPos p, int l) : Item(g, p, "xp-lvl-" + std::to_string(l) + ".png"), level(l) {
    
}

// CLEANUP
Xp::~Xp() {

}

// EVENTS
void Xp::increaseLevel() {
    static int maxXpLevel = game->getData()->others->maxXpLevel;

    if (level >= maxXpLevel) return;
    level++;
    setTexture("xp-lvl-" + std::to_string(level) + ".png");
}

// GETTER
int Xp::getLevel() const {
    return level;
}
