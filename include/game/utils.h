#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <optional>
#include <iostream>
#include <string>
#include <SDL2/SDL_types.h>
#include <SDL2/SDL_render.h>
struct LevelRect;
struct LevelSize;

enum GameState {
    MAIN_MENU,
    PLAYING,
};

enum Face {
    NONE,
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
};

enum SpriteAnimation {
    IDLE,
    WALK,
};

struct LevelPos {
    float x;
    float y;

    LevelPos operator+(const LevelPos& other) const;
    LevelPos operator-(const LevelPos& other) const;
    LevelPos operator/(const int& other) const;
    bool operator==(const LevelPos& other) const;
    LevelRect makeRect(LevelSize size) const;
};

struct LevelPosHash {
    std::size_t operator()(const LevelPos& pos) const;
};

struct LevelSize {
    float w;
    float h;

    LevelSize operator+(const LevelSize& other) const;
    LevelSize operator/(const int& other) const;
    LevelRect makeRect(LevelPos pos) const;
};

struct LevelVelocity {
    float x;
    float y;
};

struct LevelRect {
    float x;
    float y;
    float w;
    float h;

    LevelRect operator+(const LevelPos& other) const;
    Face collides(const LevelRect& other) const;
    LevelPos getPos() const;
    LevelSize getSize() const;
    LevelPos getCenter() const;
};

struct Texture {
    std::string name;
    SDL_RendererFlip flip;
};

struct Tile {
    Texture ground;
    std::optional<Texture> decoration;
};

struct TextureProbability {
    float probability;
    std::string texture;
};

struct EntityStats {
    float health;
    float attack;
    float defence;
    float speed;
    float crit;
    float pickup;
    float regeneration;
};

struct EntityData {
    Texture texture;
    LevelRect hitbox;
    EntityStats stats;
};

struct BulletData {
    Texture texture;
    LevelSize size;
    float speed;
    float damage;
    bool piercing;
    bool explosive;
};

struct WeaponData {
    std::string name;
    Texture texture;
    int delay;
    BulletData bullet;
};

std::ostream& operator<<(std::ostream& os, const LevelPos& pos);
std::ostream& operator<<(std::ostream& os, const LevelSize& size);
std::ostream& operator<<(std::ostream& os, const LevelRect& rect);

#endif
