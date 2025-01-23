#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "game.h"

std::string getRandomTexture(std::vector<TextureProbability> textureProbabilities);
bool getRandomBool();
int getIndex(float yPos, int index);
bool rand(float value);
float rand(float min, float max);
float lerp(float min, float max, float blend);

#endif
