#include "sprite.h"

// INIT
Sprite::Sprite(Game* g, std::string t, int f) : game(g), textureName(t.substr(0, t.find(".png"))), frameDelay(f) {
    static int FPS = game->getFPS();
    nbFrameDelay = f / 1000.0f * FPS;
}

// CLEANUP
Sprite::~Sprite() {

}

// UPDATE
void Sprite::update() {
    frames++;
    if (frames >= nbFrameDelay) nextFrame();
}

void Sprite::nextFrame() {
    frames = 0;
    currentSpriteFrame++;
    if (currentSpriteFrame > 4) currentSpriteFrame = 1;
}

// GETTER
std::string Sprite::getTexture() const {
    std::unordered_map<SpriteAnimation, std::string> strAnimation = {
        { IDLE, "idle" },
        { WALK, "walk" },
    };
    return textureName + '-' + strAnimation[currentAnimation] + '-' + std::to_string(currentSpriteFrame) + ".png";
}

// SETTER
void Sprite::setAnimation(SpriteAnimation animation) {
    if (currentAnimation != animation) currentSpriteFrame = 1;
    currentAnimation = animation;
}
