#include "weapon.h"

// INIT
Weapon::Weapon(Game* g, Entity* o, WeaponData d) : game(g), owner(o), data(d) {
    static H2DE_Engine* engine = game->getEngine();

    shotTimeline = H2DE_CreateTimeline(engine, data.delay, LINEAR, NULL, [this]() {
        H2DE_LevelPos pos = { 0.0f, 0.0f };

        Enemy* enemy = dynamic_cast<Enemy*>(owner);
        Player* player = dynamic_cast<Player*>(owner);

        if (enemy) {
            Player* player = game->getMap()->getPlayer();
            H2DE_LevelPos offset = player->getData().damageHitbox.getCenter();
            pos = player->getObjectData()->pos + offset;

        } else if (player) pos = H2DE_ConvertToLevelPos(engine, game->getMousePos());
        else return;

        shot(pos);
    }, -1);
}

// CLEANUP
Weapon::~Weapon() {
    if (shotTimeline) delete shotTimeline;
    for (Bullet* bullet : bullets) delete bullet;
    std::cout << "Weapon cleared" << std::endl;
}

// EVENTS
void Weapon::shot(H2DE_LevelPos targetPos) {
    H2DE_LevelPos offset = owner->getData().damageHitbox.getCenter();
    H2DE_LevelPos startPos = owner->getObjectData()->pos + offset;

    float angle = std::atan2(targetPos.y - startPos.y, targetPos.x - startPos.x);
    bullets.push_back(new Bullet(game, this, data.bullet, angle));
}

// UPDATE
void Weapon::update() {
    H2DE_TickTimeline(shotTimeline);
    updateBullets();
    updateBulletsToRemove();
}

void Weapon::updateBullets() {
    for (Bullet* bullet : bullets) bullet->update();
}

void Weapon::updateBulletsToRemove() {
    static H2DE_Camera* camera = H2DE_GetCamera(game->getEngine());
    
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [&](Bullet* bullet) {
        bool isOnCamera = H2DE_CameraContains(camera, bullet->getData().rect + H2DE_GetObjectData(bullet->getObject())->pos);
        if (!isOnCamera) {
            delete bullet;
            return true;
        } else return false;
    }), bullets.end());

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet* bullet) {
        bool hasHit = bullet->getHit(); 
        if (hasHit) {
            delete bullet;
            return true;
        } else return false;
    }), bullets.end());
}

// GETTER
Entity* Weapon::getOwner() const {
    return owner;
}

std::vector<Bullet*> Weapon::getBullets() const {
    return bullets;
}
