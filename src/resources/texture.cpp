#include <resources/texture.hpp>
#include <stb_image.h>
#include <stb_image_write.h>
#include <utility/log.hpp>

std::optional<Image> Image::New(size_t width, size_t height, uint32_t default_val)
{
    return FromData(width, height, std::vector<uint32_t>(width * height, default_val));
}

std::optional<Image> Image::FromData(size_t width, size_t height, std::vector<uint32_t>&& data)
{
    Image out {};
    out.data = Array2D<uint32_t>(std::move(data), width, height);

    SDL_Surface* surface = SDL_CreateSurfaceFrom(width, height, SDL_PIXELFORMAT_RGBA32, out.data.Data(), 4 * width);

    if (surface == nullptr)
    {
        Log("SDL: {}", SDL_GetError());
        return std::nullopt;
    }

    out.surface = decltype(out.surface) { surface };
    return out;
}

std::optional<Image> Image::FromFile(const std::string& path)
{
    int width {}, height {}, comp {};
    auto* result = stbi_load(path.c_str(), &width, &height, &comp, 4);

    if (result == nullptr)
    {
        Log("STB Image: {}", stbi_failure_reason());
        return std::nullopt;
    }

    auto* first = std::bit_cast<uint32_t*>(result);
    auto out = FromData(width, height, std::vector<uint32_t>(first, first + width * height));
    stbi_image_free(result);

    return out;
}

void Image::CopyFrom(const Image& image, const SDL_Rect& dst_rect)
{
    glm::uvec2 src_size = { image.surface->w, image.surface->h };

    for (int j = 0; j < dst_rect.h; j++)
    {
        for (int i = 0; i < dst_rect.w; i++)
        {
            glm::uvec2 pixel_pos = { dst_rect.x + i, dst_rect.y + j };
            data.At(pixel_pos.x, pixel_pos.y) = image.data.At(i, j);
        }
    }
}

void Image::SaveAsPNG(const std::string& path)
{
    stbi_write_png(path.c_str(), surface->w, surface->h, 4, data.Data(), 4 * surface->w);
}

glm::uvec2 Image::GetSize() const
{
    return { (uint32_t)surface->w, (uint32_t)surface->h };
}

std::optional<Texture> Texture::FromImage(Renderer& renderer, const Image& image)
{
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.GetRenderer(), image.surface.get());
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    if (texture == nullptr)
    {
        Log("Texture Error: {}", SDL_GetError());
        return std::nullopt;
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    Texture out {};
    out.handle = decltype(out.handle) { texture };
    return out;
}

std::optional<Texture> Texture::FromData(Renderer& renderer, const void* data, glm::uvec2 size)
{
    SDL_Surface* surface = SDL_CreateSurfaceFrom(size.x, size.y, SDL_PIXELFORMAT_RGBA32, const_cast<void*>(data), 4 * size.x);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.GetRenderer(), surface);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    SDL_DestroySurface(surface);

    if (texture == nullptr)
    {
        Log("Texture Error: {}", SDL_GetError());
        return std::nullopt;
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    Texture out {};
    out.handle = decltype(out.handle) { texture };
    return out;
}

glm::uvec2 Texture::GetSize() const
{
    return {
        (uint32_t)handle->w, (uint32_t)handle->h
    };
}