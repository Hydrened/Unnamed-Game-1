#include "entity.h"

// INIT
Player::Player(Game* g, Map* m, LevelPos p, EntityData d) : Entity(g, m, p, d) {
    equipWeapon(1);
}

// CLEANUP
Player::~Player() {

}

// EVENTS
void Player::kill() {
    
}

void Player::increaseXp(int level) {
    xp += level;
    std::cout << "XP: " << xp << std::endl;
}

void Player::increaseCoins(int nb) {
    coins += nb;
    std::cout << "COINS: " << coins << std::endl;
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
    updateForItems();
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

    velocity = { dx * data.stats.speed, dy * data.stats.speed };
    pos = pos + velocity;
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

void Player::updateForItems() {
    static GameData* gameData = game->getData();
    static std::unordered_map<std::string, LevelRect> itemHitboxes = gameData->physics->itemHitboxes;
    static LevelRect defaultPlayerHitbox = gameData->others->entities[0].hitbox;

    LevelPos playerPosCenter = (defaultPlayerHitbox + pos).getCenter();

    for (Item* item : map->getItems()) {
        LevelRect defaultItemHitbox = itemHitboxes[item->getTexture()];
        LevelPos itemPosCenter = (defaultItemHitbox + item->getPos()).getCenter();
        LevelPos posDistance = playerPosCenter - itemPosCenter;
        float distanceWithItem = std::abs(posDistance.x) + std::abs(posDistance.y);
        if (distanceWithItem <= data.stats.pickup) item->pickUp();
    }
}
