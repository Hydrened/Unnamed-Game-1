#ifndef H2DE_GRAPHIC_OBJECTS_H
#define H2DE_GRAPHIC_OBJECTS_H

#include <SDL2/SDL_types.h>
#include <H2DE/H2DE_types.h>
#include <functional>
#include <optional>
#include <string>
#include <vector>

/**
 * Type used to store the data for a graphic object
 * \since H2DE-1.0.0
 */
struct H2DE_GraphicObject {
    H2DE_GraphicObjectType type;                                    /**< Type of the object */

    H2DE_Pos pos = { 0, 0 };                                        /**< Position of the object */
    H2DE_Size size = { 0, 0 };                                      /**< Size of the object (only for `H2DE_GraphicObjectType::IMAGE`) */
    std::vector<SDL_Point> points;                                  /**< Points of the polygon (only for `H2DE_GraphicObjectType::POLYGON`) */
    unsigned int radius = 0;                                        /**< Radius of the circle (only for `H2DE_GraphicObjectType::CIRCLE`) */
    std::optional<H2DE_Rect> srcRect = std::nullopt;                /**< Part of the image to display (only for `H2DE_GraphicObjectType::IMAGE`) */

    std::string texture = "";                                       /**< Texture of the object (only for `H2DE_GraphicObjectType::IMAGE`) */

    H2DE_Pos rotationOrigin = { 0, 0 };                             /**< Position of the rotation point */
    float rotation = 0.0f;                                          /**< Rotation of the object */

    H2DE_Pos scaleOrigin = { 0, 0 };                                /**< Position of the scale point */
    H2DE_Scale scale = { 1.0f, 1.0f };                              /**< Scale multiplier for the object */

    SDL_RendererFlip flip = SDL_FLIP_NONE;                          /**< Flip the object */

    H2DE_RGB color = { 255, 255, 255, 255 };                        /**< Color of the object */
    bool filled = false;                                            /**< If you need to fill the object (only for `H2DE_GraphicObjectType::POLYGON` and `H2DE_GraphicObjectType::CIRCLE`) */

    bool repeatX = false;                                           /**< If you need the object to repeat on the x axis */
    bool repeatY = false;                                           /**< If you need the object to repeat on the y axis */

    H2DE_GraphicObject* parent = nullptr;                           /**< Parent of the object */
    int index = 0;                                                  /**< The order of the object */

    std::optional<std::function<void()>> onclick = std::nullopt;    /**< Function that'll be triggered when clicked */
};

/**
 * Creates a graphic object
 * \since H2DE-1.1.4
 */
extern H2DE_GraphicObject* H2DE_CreateGraphicObject();
/**
 * Creates a copy of a graphic object
 * \param g a graphic object
 * \since H2DE-1.1.4
 */
extern H2DE_GraphicObject* H2DE_CreateGraphicObject(H2DE_GraphicObject g);
/**
 * Destroys a graphic object
 * \param g the graphic object to destroy
 * \since H2DE-1.0.0
 */
extern void H2DE_DestroyGraphicObject(H2DE_GraphicObject* g);

#endif
