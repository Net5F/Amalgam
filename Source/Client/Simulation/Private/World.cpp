#include "World.h"
#include "TileIndex.h"
#include "BoundingBox.h"
#include "Position.h"
#include "MovementHelpers.h"
#include "Log.h"

namespace AM
{
namespace Client
{
World::World(ResourceManager& inResourceManager)
: playerEntity(entt::null)
, mapLayers(3)
, mouseScreenPoint{}
, resourceManager(inResourceManager)
{
    // Init our layers.
    for (TileLayer& layer : mapLayers) {
        layer.resize(WORLD_WIDTH * WORLD_HEIGHT);
    }

    // Fill layer 0 (terrain layer).
    // TODO: Load this from some storage format.
    SDL2pp::Rect floorPosInTexture{(256 * 6), 0, 256, 512};
    entt::resource_handle<SDL2pp::Texture> textureHandle
        = resourceManager.getTexture("iso_test_sprites.png");
    std::fill(mapLayers[0].begin(), mapLayers[0].end(),
              Sprite{textureHandle, floorPosInTexture, 256, 512});

    // Add some rugs to layer 1.
    SDL2pp::Rect rugPosInTexture{(256 * 6), (512 * 1), 256, 512};
    addTile(1, {0, 3}, textureHandle, rugPosInTexture, BoundingBox{});
    addTile(1, {4, 3}, textureHandle, rugPosInTexture, BoundingBox{});
    addTile(1, {3, 6}, textureHandle, rugPosInTexture, BoundingBox{});
    addTile(1, {9, 2}, textureHandle, rugPosInTexture, BoundingBox{});
    addTile(1, {1, 5}, textureHandle, rugPosInTexture, BoundingBox{});

    // Add some walls to layer 2.
    SDL2pp::Rect wallPosInTexture{(256 * 8), (512 * 1), 256, 512};
    addTile(2, {2, 0}, textureHandle, wallPosInTexture,
            BoundingBox{0, 13.5, 0, 32, 0, 73});
    addTile(2, {2, 1}, textureHandle, wallPosInTexture,
            BoundingBox{0, 13.5, 0, 32, 0, 73});
    addTile(2, {2, 2}, textureHandle, wallPosInTexture,
            BoundingBox{0, 13.5, 0, 32, 0, 73});

    SDL2pp::Rect wallPosInTexture2{(256 * 8), (512 * 2), 256, 512};
    addTile(2, {0, 2}, textureHandle, wallPosInTexture2,
            BoundingBox{0, 32, 0, 13.5, 0, 73});
}

void World::addTile(unsigned int layer, const TileIndex& index,
                    entt::resource_handle<SDL2pp::Texture> textureHandle,
                    const SDL2pp::Rect& extent, const BoundingBox& modelBounds)
{
    // Convert the 2d tile position into an index into the 1d array.
    unsigned int linearizedIndex = index.y * WORLD_WIDTH + index.x;
    Sprite& sprite = mapLayers[layer][linearizedIndex];

    // Build the sprite.
    sprite.textureHandle = textureHandle;
    sprite.width = TILE_SPRITE_WIDTH;
    sprite.height = TILE_SPRITE_HEIGHT;
    sprite.textureExtent = extent;
    sprite.modelBounds = modelBounds;

    // Move the sprite's world bounds to match the tile's world position.
    Position tilePosition{static_cast<float>(index.x * TILE_WORLD_WIDTH),
                          static_cast<float>(index.y * TILE_WORLD_HEIGHT), 0};
    MovementHelpers::moveSpriteWorldBounds(tilePosition, sprite);
}

} // namespace Client
} // namespace AM
