#pragma once
#include <optional>
#include <string>
#include <engine/renderer.hpp>
#include <utility/memory.hpp>
#include <utility/array2D.hpp>
#include <glm/vec2.hpp>

// RGBA 32 Image
class Image
{
public:
    static std::optional<Image> New(size_t width, size_t height, uint32_t default_val = 0);
    static std::optional<Image> FromData(size_t width, size_t height, std::vector<uint32_t>&& data);
    static std::optional<Image> FromFile(const std::string& path);

    void CopyFrom(const Image& source, const SDL_Rect& dst_rect);
    void SaveAsPNG(const std::string& path);

    glm::uvec2 GetSize() const;
    uint32_t GetPixel(uint32_t x, uint32_t y) const;

    Array2D<uint32_t> data {};
    utility::unique_ptr<SDL_Surface, SDL_DestroySurface> surface;
};

// RGBA 8 bits per channel
class Texture
{
public:
    static std::optional<Texture> FromData(Renderer& renderer, const void* data, glm::uvec2 size);

    static std::optional<Texture> FromImage(Renderer& renderer, const Image& image);

    static std::shared_ptr<Texture> SharedFromImage(Renderer& renderer, const Image& image)
    {
        if (auto texture = FromImage(renderer, image))
            return std::make_shared<Texture>(std::move(texture.value()));
        return nullptr;
    }

    static std::optional<Texture> FromFile(Renderer& renderer, const std::string& path)
    {
        if (auto image = Image::FromFile(path))
            return FromImage(renderer, image.value());
        return std::nullopt;
    }

    static std::shared_ptr<Texture> SharedFromFile(Renderer& renderer, const std::string& path)
    {
        if (auto texture = FromFile(renderer, path))
            return std::make_shared<Texture>(std::move(texture.value()));
        return nullptr;
    }

    glm::uvec2 GetSize() const;

    utility::unique_ptr<SDL_Texture, SDL_DestroyTexture> handle {};
};
