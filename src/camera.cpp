#include "camera.h"

// INIT
Camera::Camera(Game* g) : game(g) {

}

// CLEANUP
Camera::~Camera() {

}

// UPDATE
void Camera::update() {
    static GameData* gameData = game->getData();
    static Player* player = game->getMap()->getPlayer();

    LevelPos playerPos = player->getPos();
    pos.x = playerPos.x - (gameData->sizes->blocksOnWidth / 2.0f);
    pos.y = playerPos.y - (gameData->sizes->blocksOnHeight / 2.0f);
}

// RENDER
void Camera::render() {

}

// EVENTS
bool Camera::contains(LevelRect objectRect) {
    static LevelSize size = { game->getData()->sizes->blocksOnWidth, game->getData()->sizes->blocksOnHeight };
    
    LevelRect cameraRect = pos.makeRect(size + LevelSize{ 0.0f, 1.0f });
    return (cameraRect.collides(objectRect) != NONE);
}

// GETTER
LevelPos Camera::getPos() const {
    return pos;
}

// SETTER
void Camera::setPos(LevelPos p) {
    pos = p;
}
