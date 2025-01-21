#ifndef SPRITE_H
#define SPRITE_H

#include "game.h"
class Game;

class Sprite {
private:
    Game* game;
    std::string textureName;
    int frameDelay;

    int nbFrameDelay;
    int frames = 0;

    SpriteAnimation currentAnimation = IDLE;
    int currentSpriteFrame = 1;

    void nextFrame();

public:
    Sprite(Game* game, std::string texture, int frameDelay);
    ~Sprite();

    void update();

    std::string getTexture() const;

    void setTexture(std::string texture);
    void setAnimation(SpriteAnimation animation);
};

#endif
