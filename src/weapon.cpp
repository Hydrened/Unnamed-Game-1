#include "weapon.h"

// INIT
Weapon::Weapon(Game* g, Entity* u, WeaponData d) : game(g), user(u), data(d) {

}

// CLEANUP
Weapon::~Weapon() {
    for (Bullet* bullet : bullets) delete bullet;
    bullets.clear();
    bulletsToRemove.clear();
}

// EVENTS
void Weapon::shot() {
    static Calculator* cal = game->getCalculator();

    int x, y;
    game->getMousePos(&x, &y);
    LevelPos cursorPos = cal->computePxPos(x, y);
    LevelPos playerPos = game->getMap()->getPlayer()->getPos();

    bullets.push_back(new Bullet(game, this, data.bullet, std::atan2(cursorPos.y - playerPos.y, cursorPos.x - playerPos.x)));
}

void Weapon::destroyBullet(Bullet* bullet) {
    if (std::find(bulletsToRemove.begin(), bulletsToRemove.end(), bullet) == bulletsToRemove.end()) bulletsToRemove.push_back(bullet);
}

// UPDATE
void Weapon::update() {
    updateShooting();
    updateBullets();
}

void Weapon::updateShooting() {
    if (!canShoot) return;

    std::vector<int> mouseButtonsDown = game->getMouseButtonsDown();
    if (std::find(mouseButtonsDown.begin(), mouseButtonsDown.end(), 1) != mouseButtonsDown.end()) shooting = true;
    else shooting = false;

    if (shooting) {
        canShoot = false;
        Game::delay(data.delay, [this]() {
            canShoot = true;
        });
        shot();
    }
}

void Weapon::updateBullets() {
    for (Bullet* bullet : bullets) bullet->update();

    for (Bullet* bullet : bulletsToRemove) {
        auto it = std::find(bullets.begin(), bullets.end(), bullet);
        if (it != bullets.end()) {   
            delete *it;
            bullets.erase(it);
        }
    }
    bulletsToRemove.clear();
}

// RENDER
void Weapon::render() {
    for (Bullet* bullet : bullets) bullet->render();
}

// GETTER
Entity* Weapon::getUser() const {
    return user;
}
