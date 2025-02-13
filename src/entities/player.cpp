#include "entity.h"

// INIT
Player::Player(Game* g, Map* m, H2DE_LevelPos p, EntityData d) : Entity(g, m, p, d) {
    getObjectData()->hitboxes.at(1).onCollide = [this](H2DE_LevelObject* object) {
        Enemy* enemy = map->getEnemy(object);
        Bullet* bullet = map->getBullet(object);

        if (enemy) {
            EntityData enemyData = enemy->getData();

            if (enemy->canAttack()) {
                enemy->attacked();
                inflictDamages(enemyData.stats.attack, enemyData.stats.crit);
            }
            
        } else if (bullet) {
            Enemy* enemy = dynamic_cast<Enemy*>(bullet->getWeapon()->getOwner());

            if (enemy) {
                EntityData enemyData = enemy->getData();
                inflictDamages(bullet->getData().damage, enemyData.stats.crit);
            }
        }
    };

    equipWeapon(0);
}

// CLEANUP
Player::~Player() {
    std::cout << "Player cleared" << std::endl;
}

// EVENTS
void Player::killImpl() {
    
}

void Player::increaseXp(int level) {
    xp += level;
}

void Player::increaseCoins(int nb) {
    coins += nb;
}

// UPDATE
void Player::updateImpl() {
    updateForControls();
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
}

void Player::updateFacingImpl() {
    static H2DE_Engine* engine = game->getEngine();
    H2DE_LevelPos mousePos = H2DE_ConvertToLevelPos(engine, game->getMousePos());
    H2DE_LevelObjectData* objData = H2DE_GetObjectData(object);

    if (mousePos.x <= objData->pos.x + objData->texture->getData()->size.w / 2) facing = H2DE_LEFT_FACE;
    else facing = H2DE_RIGHT_FACE;
}

void Player::updateForItems() {
    H2DE_LevelPos playerCenter = (getObjectData()->hitboxes.at(0).rect + getObjectData()->pos).getCenter();

    for (Item* item : map->getItems()) {
        H2DE_LevelObjectData* itemData = item->getObjectData();
        H2DE_LevelPos itemCenter = (itemData->hitboxes.at(0).rect + itemData->pos).getCenter();

        H2DE_LevelPos posDistance = playerCenter - itemCenter;
        posDistance.x = std::abs(posDistance.x);
        posDistance.y = std::abs(posDistance.y);

        float distance = std::sqrt(std::pow(posDistance.x, 2) + std::pow(posDistance.y, 2));
        if (distance <= data.stats.pickup) item->pickUp();
    }
}
