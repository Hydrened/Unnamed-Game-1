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

void Xp::collides() {
    Map* map = game->getMap();
    map->removeItem(this);
    map->getPlayer()->increaseXp(level);
}

// UPDATE
void Xp::updateImpl() {
    updateMerge();
}

void Xp::updateMerge() {
    static int maxXpLevel = game->getData()->others->maxXpLevel;
    static std::unordered_map<std::string, LevelRect> itemHitboxes = game->getData()->physics->itemHitboxes;

    Map* map = game->getMap();
    if (map->hasToBeRemoved(this)) return;
    LevelRect xpHitbox = itemHitboxes[texture] + pos;

    for (Item* item : map->getItems()) {
        Xp* other = dynamic_cast<Xp*>(item);
        if (!other) continue; 

        if (other == this) continue;
        if (map->hasToBeRemoved(other)) continue;
        if (level >= maxXpLevel) continue;
        if (level != other->getLevel()) continue;

        LevelRect otherHitbox = itemHitboxes[other->getTexture()] + other->getPos();
        if (xpHitbox.collides(otherHitbox) == NONE) continue;

        if (velocity > other->getVelocity()) {
            increaseLevel();
            map->removeItem(other);
        } else {
            other->increaseLevel();
            map->removeItem(this);
        }
        break;
    }
}

// GETTER
int Xp::getLevel() const {
    return level;
}
