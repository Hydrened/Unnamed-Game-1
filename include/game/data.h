#ifndef DATA_H
#define DATA_H

struct GameData {
    std::unordered_map<std::string, TextureData> texturesData = {
        { "ground.png", { { 0.0f, 0.0f }, { 1.0f, 1.0f } } },
        { "bush.png", { { -0.5f, 0.0f }, { 2.0f, 1.0f } } },
    };

    std::unordered_map<H2DE_LevelPos, TileData> mapData = {
        {{ 0.0f, 0.0f }, { TileGround{ "ground.png" }, std::nullopt }},
        {{ 1.0f, 0.0f }, { TileGround{ "ground.png" }, std::nullopt }},
        {{ 2.0f, 0.0f }, { TileGround{ "ground.png" }, std::nullopt }},
        {{ 3.0f, 0.0f }, { TileGround{ "ground.png" }, TileDecoration{ "bush.png", H2DE_Hitbox{{ 0.0f, 0.0f, 1.0f, 1.0f }, { 255, 0, 0, 255 }, 0, true, std::nullopt }}}},
        {{ 4.0f, 0.0f }, { TileGround{ "ground.png" }, std::nullopt }},
    };

    std::unordered_map<int, EntityData> entitiesData = { // health, attack, defence, speed, crit, pickup, regeneration
        { 0, { "player.png", { 10.0f, 1.0f, 0.0f, 0.05f, 5.0f, 1.0f, 0.0f }, H2DE_LevelRect{ 0.0f, 0.0f, 1.0f, 1.0f }, H2DE_LevelRect{ 0.25f, 0.25f, 0.5f, 0.5f }}},
        { 1, { "player.png", { 10.0f, 1.0f, 0.0f, 0.02f, 5.0f, 1.0f, 0.0f }, std::nullopt, H2DE_LevelRect{ 0.25f, 0.25f, 0.5f, 0.5f }}},
    };

    std::unordered_map<int, WeaponData> weaponsData = { // speed, damage, piercing, explosive
        { 0, { "9mm", "weapon.png", 500, {
            "9mm-bullet.png", { 0.0f, 0.0f, 0.15f, 0.15f },
                0.15f, 10.0f, false, false
            }
        }},
        { 1, { "m4", "weapon.png", 200, {
            "9mm-bullet.png", { 0.0f, 0.0f, 0.15f, 0.15f },
                0.2f, 5.0f, false, false
            }
        }},
    };

    float nearPlayer = 0.2f;
    float critDamageMultiplier = 1.5f;
    int enemyAttackDelay = 600;
};

#endif
