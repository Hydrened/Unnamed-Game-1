#include "interface.h"

// INIT
Interface::Interface(Game* g, Map* m) : game(g), map(m) {
    initTimeLeft();
    initHealthBar();
    initXpBar();
    initKillCount();
}

void Interface::initTimeLeft() {
    static H2DE_Engine* engine = game->getEngine();
    static H2DE_Camera* camera = H2DE_GetCamera(engine);
    static float cameraPadding = game->getData()->cameraPadding;

    H2DE_LevelSize camSize = H2DE_GetCameraSize(camera);

    H2DE_LevelObjectData timeLeftObjData = H2DE_LevelObjectData();
    timeLeftObjData.pos = { camSize.w / 2, cameraPadding };
    timeLeftObjData.absolute = true;
    timeLeftObjData.index = 1000;

    H2DE_TextData textData = H2DE_TextData();
    textData.text = std::to_string(timeLeft);
    textData.fontName = "test";
    textData.charSize = { 0.35f, 0.49f };
    textData.spacing = 0.1f;
    textData.color = { 255, 255, 255, 255 };
    textData.textAlign = H2DE_TEXT_ALIGN_CENTER;
    timeLeftObjData.texture = H2DE_CreateText(engine, textData);

    timeObj = H2DE_CreateLevelObject(engine, timeLeftObjData);

    timeLeftTimeline = H2DE_CreateTimeline(engine, 1000, LINEAR, NULL, [this]() {
        timeLeft -= 1;
    }, 1200);
}

void Interface::initHealthBar() {
    static H2DE_Engine* engine = game->getEngine();
    static H2DE_LevelSize healthBarSize = game->getData()->healthBarSize;
    static float cameraPadding = game->getData()->cameraPadding;
    static float innerPadding = game->getData()->innerPadding;



    H2DE_LevelObjectData healthBarBackgroundData = H2DE_LevelObjectData();
    healthBarBackgroundData.pos = { cameraPadding, cameraPadding };
    healthBarBackgroundData.absolute = true;
    healthBarBackgroundData.index = 1000;

    H2DE_TextureData healthBarBackgroundTextureData = H2DE_TextureData();
    healthBarBackgroundTextureData.name = "bar.png";
    healthBarBackgroundTextureData.size = healthBarSize;
    healthBarBackgroundTextureData.scaleMode = H2DE_SCALE_MODE_NEAREST;
    healthBarBackgroundTextureData.color = { 255, 0, 0, 255 };
    healthBarBackgroundData.texture = H2DE_CreateTexture(engine, healthBarBackgroundTextureData);

    healthBarBackground = H2DE_CreateLevelObject(engine, healthBarBackgroundData);



    H2DE_LevelObjectData healthBarData = H2DE_LevelObjectData();
    healthBarData.pos = { cameraPadding, cameraPadding };
    healthBarData.absolute = true;
    healthBarData.index = 1000;

    H2DE_TextureData healthBarTextureData = H2DE_TextureData();
    healthBarTextureData.name = "bar.png";
    healthBarTextureData.size = healthBarSize;
    healthBarTextureData.scaleMode = H2DE_SCALE_MODE_NEAREST;
    healthBarTextureData.color = { 34, 177, 76, 255 };
    healthBarData.texture = H2DE_CreateTexture(engine, healthBarTextureData);

    healthBar = H2DE_CreateLevelObject(engine, healthBarData);



    H2DE_LevelObjectData healthIndicatorData = H2DE_LevelObjectData();
    healthIndicatorData.pos = { cameraPadding + healthBarSize.w + innerPadding, cameraPadding + 0.06f };
    healthIndicatorData.absolute = true;
    healthIndicatorData.index = 1000;

    H2DE_TextData healthIndicatorTextData = H2DE_TextData();
    healthIndicatorTextData.text = "";
    healthIndicatorTextData.fontName = "test";
    healthIndicatorTextData.charSize = { 0.2f, 0.28f };
    healthIndicatorTextData.spacing = 0.03f;
    healthIndicatorTextData.color = { 255, 255, 255, 255 };
    healthIndicatorTextData.textAlign = H2DE_TEXT_ALIGN_LEFT;
    healthIndicatorData.texture = H2DE_CreateText(engine, healthIndicatorTextData);

    healthIndicator = H2DE_CreateLevelObject(engine, healthIndicatorData);
    refreshHealthBar();
}

void Interface::initXpBar() {
    static H2DE_Engine* engine = game->getEngine();
    static H2DE_LevelSize healthBarSize = game->getData()->healthBarSize;
    static H2DE_LevelSize xpBarSize = game->getData()->xpBarSize;
    static float cameraPadding = game->getData()->cameraPadding;
    static float innerPadding = game->getData()->innerPadding;

    H2DE_LevelPos xpBarPos = { cameraPadding, cameraPadding + innerPadding + healthBarSize.h };



    H2DE_LevelObjectData xpBarBackgroundData = H2DE_LevelObjectData();
    xpBarBackgroundData.pos = xpBarPos;
    xpBarBackgroundData.absolute = true;
    xpBarBackgroundData.index = 1000;

    H2DE_TextureData xpBarBackgroundTextureData = H2DE_TextureData();
    xpBarBackgroundTextureData.name = "bar.png";
    xpBarBackgroundTextureData.size = xpBarSize;
    xpBarBackgroundTextureData.scaleMode = H2DE_SCALE_MODE_NEAREST;
    xpBarBackgroundTextureData.color = { 0, 27, 50, 255 };
    xpBarBackgroundData.texture = H2DE_CreateTexture(engine, xpBarBackgroundTextureData);

    xpBarBackground = H2DE_CreateLevelObject(engine, xpBarBackgroundData);



    H2DE_LevelObjectData xpBarData = H2DE_LevelObjectData();
    xpBarData.pos = xpBarPos;
    xpBarData.absolute = true;
    xpBarData.index = 1000;

    H2DE_TextureData xpBarTextureData = H2DE_TextureData();
    xpBarTextureData.name = "bar.png";
    xpBarTextureData.size = { 0.0f, xpBarSize.h };
    xpBarTextureData.scaleMode = H2DE_SCALE_MODE_NEAREST;
    xpBarTextureData.color = { 0, 162, 232, 255 };
    xpBarData.texture = H2DE_CreateTexture(engine, xpBarTextureData);

    xpBar = H2DE_CreateLevelObject(engine, xpBarData);



    H2DE_LevelObjectData levelIndicatorData = H2DE_LevelObjectData();
    levelIndicatorData.pos = { xpBarPos.x + xpBarSize.w + innerPadding, xpBarPos.y + 0.06f };
    levelIndicatorData.absolute = true;
    levelIndicatorData.index = 1000;

    H2DE_TextData levelIndicatorTextData = H2DE_TextData();
    levelIndicatorTextData.fontName = "test";
    levelIndicatorTextData.charSize = { 0.2f, 0.28f };
    levelIndicatorTextData.spacing = 0.03f;
    levelIndicatorTextData.color = { 255, 255, 255, 255 };
    levelIndicatorTextData.textAlign = H2DE_TEXT_ALIGN_LEFT;
    levelIndicatorData.texture = H2DE_CreateText(engine, levelIndicatorTextData);

    levelIndicator = H2DE_CreateLevelObject(engine, levelIndicatorData);
    refreshXpBar();
}

void Interface::initKillCount() {
    static H2DE_Engine* engine = game->getEngine();
    static H2DE_Camera* camera = H2DE_GetCamera(engine);
    static float cameraPadding = game->getData()->cameraPadding;

    H2DE_LevelSize camSize = H2DE_GetCameraSize(camera);

    H2DE_LevelObjectData killCountObjData = H2DE_LevelObjectData();
    killCountObjData.pos = { camSize.w - cameraPadding, cameraPadding };
    killCountObjData.absolute = true;
    killCountObjData.index = 1000;

    H2DE_TextData textData = H2DE_TextData();
    textData.text = "0";
    textData.fontName = "test";
    textData.charSize = { 0.35f, 0.49f };
    textData.spacing = 0.1f;
    textData.color = { 255, 255, 255, 255 };
    textData.textAlign = H2DE_TEXT_ALIGN_RIGHT;
    killCountObjData.texture = H2DE_CreateText(engine, textData);

    killCount = H2DE_CreateLevelObject(engine, killCountObjData);
}

//CLEANUP
Interface::~Interface() {
    destroyTimeLeftTimeline();
    destroyHealthBar();
    destroyXpBar();
    destroyKillCount();
    if (game->isDebuging()) std::cout << "Interface cleared" << std::endl;
}

void Interface::destroyTimeLeftTimeline() {
    if (timeLeftTimeline) delete timeLeftTimeline;
}

void Interface::destroyHealthBar() {
    static H2DE_Engine* engine = game->getEngine();

    if (healthBarBackground) H2DE_DestroyLevelObject(engine, healthBarBackground);
    if (healthBar) H2DE_DestroyLevelObject(engine, healthBar);
    if (healthIndicator) H2DE_DestroyLevelObject(engine, healthIndicator);
}

void Interface::destroyXpBar() {
    static H2DE_Engine* engine = game->getEngine();

    if (xpBarBackground) H2DE_DestroyLevelObject(engine, xpBarBackground);
    if (xpBar) H2DE_DestroyLevelObject(engine, xpBar);
    if (levelIndicator) H2DE_DestroyLevelObject(engine, levelIndicator);
}

void Interface::destroyKillCount() {
    static H2DE_Engine* engine = game->getEngine();
    if (killCount) H2DE_DestroyLevelObject(engine, killCount);
}

// UPDATE
void Interface::update() {
    updateTimeLeft();
}

void Interface::updateTimeLeft() {
    H2DE_TickTimeline(timeLeftTimeline);
    H2DE_GetTextData(dynamic_cast<H2DE_Text*>(H2DE_GetObjectData(timeObj)->texture))->text = Interface::timeToString(timeLeft);
}

void Interface::refreshHealthBar() {
    static H2DE_LevelSize healthBarSize = game->getData()->healthBarSize;

    Player* player = map->getPlayer();
    EntityData playerData = player->getData();
    int playerHealth = player->getHealth();

    float barBlend = std::clamp(0.0f, 1.0f, static_cast<float>(playerHealth) / static_cast<float>(playerData.stats.health));
    H2DE_GetTextureData(H2DE_GetObjectData(healthBar)->texture)->size.w = healthBarSize.w * barBlend;
    H2DE_GetTextData(dynamic_cast<H2DE_Text*>(H2DE_GetObjectData(healthIndicator)->texture))->text = std::to_string(playerHealth) + '/' + std::to_string(playerData.stats.health);
}

void Interface::refreshXpBar() {
    static H2DE_LevelSize xpBarSize = game->getData()->xpBarSize;

    H2DE_GetTextureData(H2DE_GetObjectData(xpBar)->texture)->size.w = xpBarSize.w * map->getXpLevelPercentage();
    H2DE_GetTextData(dynamic_cast<H2DE_Text*>(H2DE_GetObjectData(levelIndicator)->texture))->text = "LVL " + std::to_string(map->getPlayer()->getLevel());
}

void Interface::refreshKillCount(int nb) {
    H2DE_GetTextData(dynamic_cast<H2DE_Text*>(H2DE_GetObjectData(killCount)->texture))->text = std::to_string(nb);
}

// GETTER
std::string Interface::timeToString(int time) {
    int minutes = time / 60;
    int remainingSeconds = time % 60;

    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << remainingSeconds;
    
    return ss.str();
}
