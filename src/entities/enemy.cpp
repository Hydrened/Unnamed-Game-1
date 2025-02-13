#include "entity.h"

// INIT
Enemy::Enemy(Game* g, Map* m, H2DE_LevelPos p, EntityData d) : Entity(g, m, p, d) {
    getObjectData()->hitboxes.at(0).onCollide = [this](H2DE_LevelObject* object) {
        Bullet* bullet = map->getBullet(object);

        if (bullet) {
            Player* player = dynamic_cast<Player*>(bullet->getWeapon()->getOwner());

            if (player) {
                EntityData playerData = player->getData();
                inflictDamages(bullet->getData().damage, playerData.stats.crit);
                bullet->setHit();
            }
        }
    };
}

// CLEANUP
Enemy::~Enemy() {
    std::cout << "Enemy cleared" << std::endl;
}

// EVENTS
bool Enemy::isNearPlayer() {
    static float nearPlayer = game->getData()->nearPlayer;
    H2DE_LevelPos pos = getObjectData()->pos;
    H2DE_LevelPos playerPos = map->getPlayer()->getObjectData()->pos;

    return (std::abs(playerPos.x - pos.x) < nearPlayer && std::abs(playerPos.y - pos.y) < nearPlayer);
}

void Enemy::attacked() {
    static int attackDelay = game->getData()->enemyAttackDelay;

    canAttackNow = false;
    H2DE_Engine::H2DE_Delay(attackDelay, [this]() {
        canAttackNow = true;
    });
}

bool Enemy::canAttack() const {
    return canAttackNow;
}

void Enemy::killImpl() {
    static float enemyDropingCoinProbability = game->getData()->enemyDropingCoin;
    H2DE_LevelObjectData* objData = getObjectData();

    map->dropXp(objData->pos, data.xpOnDeath);
    map->getPlayer()->increaseCoins(1);
    if (H2DE_RandomFloatInRange(0.0f, 100.0f) <= enemyDropingCoinProbability) map->dropCoin(objData->pos);
}

// UPDATE
void Enemy::updateImpl() {
    if (!isNearPlayer()) {
        static H2DE_LevelObjectData* playerData = map->getPlayer()->getObjectData();
        H2DE_LevelPos pos = getObjectData()->pos;
        H2DE_LevelPos playerPos = playerData->pos;

        float angle = std::atan2(playerPos.y - pos.y, playerPos.x - pos.x);
        velocity = { data.stats.speed * std::cos(angle), data.stats.speed * std::sin(angle) };
    } else velocity = { 0.0f, 0.0f };
}

void Enemy::updateFacingImpl() {
    if (velocity.x != 0) facing = (velocity.x > 0) ? H2DE_RIGHT_FACE : H2DE_LEFT_FACE;
}
