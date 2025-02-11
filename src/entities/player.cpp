#include "entity.h"

// INIT
Player::Player(Game* g, Map* m, H2DE_LevelPos p, EntityData d) : Entity(g, m, p, d) {
    getObjectData()->hitboxes.at(1).onCollide = [this](H2DE_LevelObject* object) {
        Enemy* enemy = map->getEnemy(object);
        if (!enemy) return;
        
        EntityData enemyData = enemy->getData();

        if (enemy->canAttack()) {
            enemy->attacked();
            inflictDamages(enemyData.stats.attack, enemyData.stats.crit);
        }
    };

    equipWeapon(0);
}

// CLEANUP
Player::~Player() {
    std::cout << "Player cleared" << std::endl;
}

// EVENTS
void Player::kill() {
    
}

// UPDATE
void Player::updateImpl() {
    updateForControls();
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
