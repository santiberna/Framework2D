#include <resources/texture_atlas.hpp>

TextureAtlas TextureAtlas::FromTextureAndTileSize(std::shared_ptr<Texture> texture, glm::uvec2 tile_size)
{
    assert(tile_size.x != 0 && tile_size.y != 0);

    std::vector<SDL_Rect> rects {};
    auto texture_size = texture->GetSize();
    SDL_Rect base_rect { 0, 0, (int)tile_size.x, (int)tile_size.y };

    for (uint32_t y = 0; y < texture_size.y; y += tile_size.y)
    {
        for (uint32_t x = 0; x < texture_size.x; x += tile_size.x)
        {
            base_rect.x = x;
            base_rect.y = y;
            rects.push_back(base_rect);
        }
    }

    return TextureAtlas::FromTextureAndRects(texture, std::move(rects));
}

TextureAtlas TextureAtlas::FromTextureAndRects(std::shared_ptr<Texture> texture, std::vector<SDL_Rect>&& rects)
{
    TextureAtlas out {};
    out.atlas = texture;
    out.rects = std::move(rects);
    return out;
}

std::optional<SDL_Rect> TextureAtlas::GetSpriteRect(size_t index) const
{
    if (index >= rects.size())
        return std::nullopt;

    return rects.at(index);
}

std::optional<SDL_FRect> TextureAtlas::GetSpriteFRect(size_t index) const
{
    if (auto res = GetSpriteRect(index))
    {
        SDL_FRect out {};
        SDL_RectToFRect(&res.value(), &out);
        return out;
    }
    return std::nullopt;
}