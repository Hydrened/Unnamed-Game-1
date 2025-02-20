#ifndef H2DE_TEXTURE_H
#define H2DE_TEXTURE_H

#include <H2DE/H2DE_engine.h>
class H2DE_Engine;

/**
 * Types used to identify surfaces
 * \since H2DE-2.0.13
 */
class H2DE_Surface {
protected:
    H2DE_Engine* engine;
    H2DE_TextureData textureData;

    std::string currentTexture;

public:
    H2DE_Surface(H2DE_Engine* engine, H2DE_TextureData textureData);
    ~H2DE_Surface();

    virtual void update() = 0;

    std::string get() const;

    /**
     * Gets the data of a texture
     * \param texture pointer to a texture
     * \returns a pointer to the texture's data
     * \since H2DE-2.1.2
     */
    friend H2DE_TextureData* H2DE_GetTextureData(H2DE_Surface* texture);
};

/**
 * Types used to identify textures
 * \since H2DE-2.0.3
 */
class H2DE_Texture : public H2DE_Surface {
public:
    H2DE_Texture(H2DE_Engine* engine, H2DE_TextureData textureData);
    ~H2DE_Texture();

    void update() override;
};

/**
 * Types used to identify sprites
 * \since H2DE-2.0.13
 */
class H2DE_Sprite : public H2DE_Surface {
private:
    H2DE_SpriteData spriteData;

    int frameDelay;
    int frames = 0;
    int currentSpriteFrame = 1;
    std::string animation;

    void setCurrentTexture();

public:
    H2DE_Sprite(H2DE_Engine* engine, H2DE_TextureData data, H2DE_SpriteData spriteData);
    ~H2DE_Sprite();

    void update() override;

    /**
     * Gets the data of a sprite
     * \param sprite pointer to a sprite
     * \returns a pointer to the sprite's data
     * \since H2DE-2.0.21
     */
    friend H2DE_SpriteData* H2DE_GetSpriteData(H2DE_Sprite* sprite);

    /**
     * Sets the animation of a sprite
     * \param sprite pointer to a sprite
     * \param animation new animation
     * \since H2DE-2.0.13
     */
    friend void H2DE_SetSpriteAnimation(H2DE_Sprite* sprite, std::string animation);
};

/**
 * Types used to identify texts
 * \since H2DE-2.1.0
 */
class H2DE_Text : public H2DE_Surface {
private:
    H2DE_TextData textData;

    H2DE_LevelSize getTextSize() const;

public:
    H2DE_Text(H2DE_Engine* engine, H2DE_TextData textData);
    ~H2DE_Text();

    void update() override;

    /**
     * Gets the data of a text
     * \param text pointer to a text
     * \returns a pointer to the text's data
     * \since H2DE-2.1.0
     */
    friend H2DE_TextData* H2DE_GetTextData(H2DE_Text* text);
};

/**
 * Creates a texture
 * \param engine pointer to the engine
 * \param textureData texture's data
 * \returns a pointer to a new texture
 * \since H2DE-2.0.13
 */
extern H2DE_Texture* H2DE_CreateTexture(H2DE_Engine* engine, H2DE_TextureData textureData);
/**
 * Creates a sprite
 * \param engine pointer to the engine
 * \param textureData texture's data
 * \param spriteData sprite's data
 * \returns a pointer to a new sprite
 * \since H2DE-2.0.13
 */
extern H2DE_Sprite* H2DE_CreateSprite(H2DE_Engine* engine, H2DE_TextureData textureData, H2DE_SpriteData spriteData);
/**
 * Creates a text
 * \param engine pointer to the engine
 * \param textData text's data
 * \returns a pointer to a new text
 * \since H2DE-2.1.0
 */
extern H2DE_Text* H2DE_CreateText(H2DE_Engine* engine, H2DE_TextData textData);

#endif
