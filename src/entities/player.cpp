#include "entity.h"

// INIT
Player::Player(Game* g, Map* m, H2DE_LevelPos p, EntityData d) : Entity(g, m, p, d) {
    H2DE_Engine* engine = game->getEngine();

    H2DE_LevelObjectData objectData = H2DE_LevelObjectData();
    objectData.pos = { 1.0f, 1.0f };

    H2DE_TextureData objectTextureData = H2DE_TextureData();
    objectTextureData.name = "player.png";
    objectTextureData.size = { 1.0f, 1.0f };
    objectTextureData.scaleMode = H2DE_SCALE_MODE_NEAREST;

    H2DE_SpriteData objectSpriteData = H2DE_SpriteData();
    objectSpriteData.defaultAnimationName = "idle";
    objectSpriteData.delay = 200;
    objectSpriteData.nbFrame = 4;

    objectData.texture = H2DE_CreateSprite(engine, objectTextureData, objectSpriteData);

    object = H2DE_CreateLevelObject(engine, objectData);
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
    // std::cout << 1 << std::endl;
}
