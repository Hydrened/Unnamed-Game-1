#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "game.h"

std::string getRandomTexture(std::vector<TextureProbability> textureProbabilities);
SDL_RendererFlip getRandomFlipX();
int getIndex(float yPos, int index);
bool rand(float value);

#endif
