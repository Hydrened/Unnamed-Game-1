#include "entity.h"

// INIT
Player::Player(Game* g, Map* m, LevelPos p, EntityData d) : Entity(g, m, p, d) {
    equipWeapon(0);
}

// CLEANUP
Player::~Player() {

}

// EVENTS
void Player::kill() {
    
}

void Player::increaseXp(int level) {
    xp += level;
    std::cout << xp << std::endl;
}

// UPDATE
void Player::update() {
    LevelPos defaultPos = pos;
    updateSprite();
    updateForControls();
    updateForWorldCollisions();
    updateFacing();
    updateAnimation(defaultPos);
    updateWeapon();
    updateForXp();
}

void Player::updateForControls() {
    float dx = 0.0f;
    float dy = 0.0f;

    for (const SDL_Keycode key : game->getKeysDown()) switch (key) {
        case SDLK_UP: dy -= 1.0f; break;
        case SDLK_DOWN: dy += 1.0f; break;
        case SDLK_LEFT: dx -= 1.0f; break;
        case SDLK_RIGHT: dx += 1.0f; break;
        default: break;
    }

    float magnitude = std::sqrt(dx * dx + dy * dy);
    if (magnitude > 0.0f) {
        dx /= magnitude;
        dy /= magnitude;
    }

    pos.x += dx * data.stats.speed;
    pos.y += dy * data.stats.speed;
}

void Player::updateForWorldCollisions() {
    static GameData* gameData = game->getData();

    static std::unordered_map<std::string, LevelRect> decorationHitboxes = gameData->physics->decorationHitboxes;
    LevelRect playerHitbox = data.hitbox + pos;

    for (const auto& [tilePos, tile] : map->getPerimeter({ pos.x + playerHitbox.w / 2, pos.y + playerHitbox.h / 2 }, true)) {
        if (!tile->decoration.has_value()) continue;
        if (decorationHitboxes.find(tile->decoration.value()) == decorationHitboxes.end()) continue;

        LevelRect decorationHitbox = decorationHitboxes[tile->decoration.value()] + tilePos;
        switch (playerHitbox.collides(decorationHitbox)) {
            case TOP: pos.y = decorationHitbox.y + decorationHitbox.h; break;
            case BOTTOM: pos.y = decorationHitbox.y - playerHitbox.h; break;
            case LEFT: pos.x = decorationHitbox.x + decorationHitbox.w; break;
            case RIGHT: pos.x = decorationHitbox.x - playerHitbox.w; break;
            case NONE: continue;
        }
    }
}

void Player::updateFacing() {
    static Calculator* cal = game->getCalculator();

    int x, y;
    game->getMousePos(&x, &y);
    LevelPos mousePos = cal->computePxPos(x, y);

    if (mousePos.x <= pos.x + data.hitbox.w / 2) facing = LEFT;
    else facing = RIGHT;
}

void Player::updateAnimation(LevelPos defaultPos) {
    sprite->setAnimation((defaultPos == pos) ? IDLE : WALK); 
}

void Player::updateForXp() {
    static LevelRect defaultPlayerHitbox = game->getData()->others->entities[0].hitbox;
    static LevelRect defaultXpHitbox = game->getData()->physics->xpHitbox;
    LevelPos playerPosCenter = (defaultPlayerHitbox + pos).getCenter();

    for (Xp* xp : map->getXps()) {
        LevelPos xpPosCenter = (defaultXpHitbox + xp->getPos()).getCenter();
        LevelPos posDistance = playerPosCenter - xpPosCenter;
        float distanceWithXp = std::abs(posDistance.x) + std::abs(posDistance.y);
        if (distanceWithXp <= data.stats.pickup) xp->pickUp();
    }
}
