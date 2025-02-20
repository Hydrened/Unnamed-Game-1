#ifndef UTILS_H
#define UTILS_H

namespace std {
    template<>
    struct hash<H2DE_LevelPos> {
        size_t operator()(const H2DE_LevelPos& pos) const {
            return hash<int>()(pos.x) ^ (hash<int>()(pos.y) << 1);
        }
    };
}

struct EntityStats {
    int health;
    int attack;
    int defence;
    float speed;
    float crit;
    float pickup;
    int regeneration;
};

struct TextureData {
    H2DE_LevelPos offset;
    H2DE_LevelSize size;
};

struct EntityData {
    std::string textureName;
    H2DE_LevelSize textureSize;
    EntityStats stats;
    std::optional<H2DE_LevelRect> collisionHitbox;
    H2DE_LevelRect damageHitbox;
    int xpOnDeath;
};

struct TileGround {
    std::string textureName;
};

struct TileDecoration {
    std::string textureName;
    std::optional<H2DE_Hitbox> hitbox;
};

struct TileData {
    TileGround ground;
    std::optional<TileDecoration> decoration;
};

struct Tile {
    TileData data;
    H2DE_LevelObject* groundObject;
    std::optional<H2DE_LevelObject*> decorationObject;
};

struct BulletData {
    std::string textureName;
    H2DE_LevelRect rect;
    float speed;
    int damage;
    bool piercing;
    bool explosive;
};

struct WeaponData {
    std::string name;
    std::string textureName;
    int delay;
    BulletData bullet;
};

#endif
