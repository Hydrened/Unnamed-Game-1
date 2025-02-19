#include "entity.h"

// INIT
Enemy::Enemy(Game* g, Map* m, H2DE_LevelPos p, EntityData d) : Entity(g, m, p, d) {
    initDamageCollision();
    initAttackTimeline();
}

void Enemy::initDamageCollision() {
    getObjectData()->hitboxes["damage"].onCollide = [this](H2DE_LevelObject* object) {
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

void Enemy::initAttackTimeline() {
    static H2DE_Engine* engine = game->getEngine();
    static int attackDelay = game->getData()->enemyAttackDelay;

    attackTimeline = H2DE_CreateTimeline(engine, attackDelay, LINEAR, NULL, [this]() {
        canAttackNow = true;
    }, 0);
}

// CLEANUP
Enemy::~Enemy() {
    if (game->isDebuging()) std::cout << "Enemy cleared" << std::endl;
}

// EVENTS
bool Enemy::isNearPlayer() {
    static float nearPlayer = game->getData()->nearPlayer;

    H2DE_LevelObjectData* thisData = getObjectData();
    H2DE_LevelObjectData* playerData = map->getPlayer()->getObjectData();

    H2DE_LevelPos pos = thisData->pos + thisData->hitboxes["damage"].rect.getCenter();
    H2DE_LevelPos playerPos = playerData->pos + playerData->hitboxes["damage"].rect.getCenter();

    return (std::abs(playerPos.x - pos.x) < nearPlayer && std::abs(playerPos.y - pos.y) < nearPlayer);
}

void Enemy::attacked() {
    canAttackNow = false;
    H2DE_ResetTimeline(attackTimeline);
}

bool Enemy::canAttack() const {
    return canAttackNow;
}

void Enemy::killImpl() {
    static float enemyDropingCoinProbability = game->getData()->enemyDropingCoin;
    H2DE_LevelObjectData* objData = getObjectData();

    map->dropXp(objData->pos + objData->hitboxes["damage"].rect.getCenter(), data.xpOnDeath);
    map->getPlayer()->increaseCoins(1);
    if (H2DE_RandomFloatInRange(0.0f, 100.0f) <= enemyDropingCoinProbability) map->dropCoin(objData->pos);

    map->enemyKilled();
}

void Enemy::inflictDamagesImpl(int damages, bool isCrit) {
    
}

// UPDATE
void Enemy::updateImpl() {
    updateVelocity();
    updateAttackTimeline();
}

void Enemy::updateVelocity() {
    if (!isNearPlayer()) {
        H2DE_LevelObjectData* thisData = getObjectData();
        H2DE_LevelObjectData* playerData = map->getPlayer()->getObjectData();

        H2DE_LevelPos pos = thisData->pos + thisData->hitboxes["damage"].rect.getCenter();
        H2DE_LevelPos playerPos = playerData->pos + playerData->hitboxes["damage"].rect.getCenter();

        float angle = std::atan2(playerPos.y - pos.y, playerPos.x - pos.x);
        velocity = { data.stats.speed * std::cos(angle), data.stats.speed * std::sin(angle) };
    } else velocity = { 0.0f, 0.0f };
}

void Enemy::updateAttackTimeline() {
    if (attackTimeline) H2DE_TickTimeline(attackTimeline);
}

void Enemy::updateFacingImpl() {
    if (velocity.x != 0) facing = (velocity.x > 0) ? H2DE_RIGHT_FACE : H2DE_LEFT_FACE;
}
