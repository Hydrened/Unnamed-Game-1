#ifndef INTERFACE_H
#define INTERFACE_H

#include "game.h"
class Game;
class Map;

class Interface {
private:
    Game* game;
    Map* map;

    H2DE_LevelObject* timeObj = nullptr;
    H2DE_Timeline* timeLeftTimeline = nullptr;
    int timeLeft = 1200;

    H2DE_LevelObject* healthBarBackground = nullptr;
    H2DE_LevelObject* healthBar = nullptr;
    H2DE_LevelObject* healthIndicator = nullptr;

    H2DE_LevelObject* xpBarBackground = nullptr;
    H2DE_LevelObject* xpBar = nullptr;
    H2DE_LevelObject* levelIndicator = nullptr;

    H2DE_LevelObject* killCount = nullptr;

    void initTimeLeft();
    void initHealthBar();
    void initXpBar();
    void initKillCount();

    void destroyTimeLeftTimeline();
    void destroyHealthBar();
    void destroyXpBar();
    void destroyKillCount();

    void updateTimeLeft();

    static std::string timeToString(int time);

public:
    Interface(Game* game, Map* map);
    ~Interface();

    void update();

    void refreshHealthBar();
    void refreshXpBar();
    void refreshKillCount(int nb);
};

#endif
