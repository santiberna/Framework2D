#pragma once
#include <resources/texture.hpp>

class TextureAtlas
{
public:
    static TextureAtlas FromTextureAndTileSize(std::shared_ptr<Texture> texture, glm::uvec2 tile_size);
    static TextureAtlas FromTextureAndRects(std::shared_ptr<Texture> texture, std::vector<SDL_Rect>&& rects);

    const Texture& GetTexture() const
    {
        return *atlas;
    }

    std::optional<SDL_Rect> GetSpriteRect(size_t index) const;
    std::optional<SDL_FRect> GetSpriteFRect(size_t index) const;

private:
    std::shared_ptr<Texture> atlas {};
    std::vector<SDL_Rect> rects {};
};