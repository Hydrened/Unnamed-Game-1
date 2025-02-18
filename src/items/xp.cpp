#include "items.h"

// INIT
Xp::Xp(Game* g, H2DE_LevelPos p, int l) : Item(g, p, "xp-lvl-" + std::to_string(l) + ".png", 2), game(g), level(l) {

}

// CLEANUP
Xp::~Xp() {
    if (game->isDebuging()) std::cout << "Xp cleared" << std::endl;
}

// EVENTS
void Xp::collidedImpl() {
    game->getMap()->getPlayer()->increaseXp(level);
}

void Xp::increaseLevel() {
    static int maxXpLevel = game->getData()->maxXpLevel;

    if (level >= maxXpLevel) return;
    level++;
    H2DE_GetTextureData(H2DE_GetObjectData(getObject())->texture)->name = "xp-lvl-" + std::to_string(level) + ".png";
}

// UPDATE
void Xp::updateImpl() {
    updateMerge();
}

void Xp::updateMerge() {
    static int maxXpLevel = game->getData()->maxXpLevel;

    if (toRemove()) return;
    if (level >= maxXpLevel) return;
    if (pickedUp) return;

    H2DE_LevelObjectData* objData = H2DE_GetObjectData(getObject());
    H2DE_LevelRect xpHitbox = objData->hitboxes["collision"].rect + objData->pos;

    for (Item* item : game->getMap()->getItems()) {
        Xp* other = dynamic_cast<Xp*>(item);
        if (!other) continue;
        if (other == this) continue;
        if (other->toRemove()) continue;
        if (other->pickedUp) continue;

        if (other->getLevel() >= maxXpLevel) continue;
        if (other->getLevel() != level) continue;
    
        H2DE_LevelObjectData* otherObjData = H2DE_GetObjectData(other->getObject());
        H2DE_LevelRect otherHitbox = otherObjData->hitboxes["collision"].rect + otherObjData->pos;
        if (xpHitbox.collides(otherHitbox) == H2DE_NO_FACE) continue;

        if (getVelocity() > other->getVelocity()) {
            increaseLevel();
            other->remove = true;
        } else {
            other->increaseLevel();
            remove = true;
        }
        break;
    }
}

// GETTER
int Xp::getLevel() const {
    return level;
}
