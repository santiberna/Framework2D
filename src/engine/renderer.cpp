#include <engine/renderer.hpp>
#include <resources/texture.hpp>
#include <engine/common.hpp>

SDL_FRect MapRect(const glm::mat3& projection, const SDL_FRect& rect)
{
    auto pos = projection * glm::vec3 { rect.x, rect.y, 1.0f };
    auto size = projection * glm::vec3 { rect.w, rect.h, 0.0f };

    return {
        pos.x, pos.y, size.x, size.y
    };
}

SDL_FRect FromCenter(const glm::vec2& center, const glm::vec2& size)
{
    return {
        center.x - size.x * 0.5f,
        center.y - size.y * 0.5f,
        size.x,
        size.y
    };
}

void Renderer::ClearScreen(const glm::vec3& colour)
{
    // Clear with borders
    SDL::AbortIfFailed(SDL_SetRenderDrawColorFloat(handle.get(), colour.r, colour.g, colour.b, 1.0f));
    SDL::AbortIfFailed(SDL_RenderClear(handle.get()));
}

void Renderer::RenderTextureRect(
    const Texture& sprite,
    const SDL_FRect& dst,
    const SDL_FRect* src,
    const glm::vec4& colour,
    bool flipY)
{
    SDL::AbortIfFailed(SDL_SetTextureColorModFloat(sprite.handle.get(), colour.x, colour.y, colour.z));
    SDL::AbortIfFailed(SDL_SetTextureAlphaModFloat(sprite.handle.get(), colour.w));

    SDL_FlipMode flip = flipY ? SDL_FlipMode::SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    SDL::AbortIfFailed(SDL_RenderTextureRotated(
        handle.get(),
        sprite.handle.get(),
        src, &dst, 0.0f,
        nullptr, flip));
}

void Renderer::RenderFilledRect(
    const SDL_FRect& dst,
    const glm::vec4& colour)
{
    SDL::AbortIfFailed(SDL_SetRenderDrawColorFloat(handle.get(), colour.x, colour.y, colour.z, colour.w));
    SDL::AbortIfFailed(SDL_RenderFillRect(handle.get(), &dst));
}

void Renderer::RenderRect(
    const glm::vec2& position,
    const glm::vec2& size,
    const glm::vec4& colour)
{
    SDL_FRect dst = FromCenter(position, size);
    RenderRect(dst, colour);
}

void Renderer::RenderRect(
    const SDL_FRect& dst,
    const glm::vec4& colour)
{
    SDL::AbortIfFailed(SDL_SetRenderDrawColorFloat(handle.get(), colour.x, colour.y, colour.z, colour.w));
    SDL::AbortIfFailed(SDL_RenderRect(handle.get(), &dst));
}

void Renderer::RenderLine(
    const glm::vec2& p1,
    const glm::vec2& p2,
    const glm::vec4& colour)
{

    SDL_SetRenderDrawColorFloat(handle.get(), colour.x, colour.y, colour.z, colour.w);
    SDL_RenderLine(handle.get(), p1.x, p1.y, p2.x, p2.y);
}