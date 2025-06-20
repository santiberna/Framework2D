#pragma once
#include <utility/memory.hpp>
#include <math/types.hpp>
#include <SDL3/SDL_render.h>

class Texture;

class Renderer
{
public:
    Renderer(SDL_Renderer* renderer)
        : handle(renderer)
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    }

    SDL_Renderer* GetRenderer() const { return handle.get(); }

    void ClearScreen(const glm::vec3& colour);

    void RenderTextureRect(
        const Texture& sprite,
        const SDL_FRect& dst,
        const SDL_FRect* src = nullptr,
        const glm::vec4& colour = { 1.0f, 1.0f, 1.0f, 1.0f },
        bool flipY = false);

    void RenderRect(
        const glm::vec2& position,
        const glm::vec2& size,
        const glm::vec4& colour);

    void RenderRect(
        const SDL_FRect& dst,
        const glm::vec4& colour);

    void RenderFilledRect(
        const SDL_FRect& dst,
        const glm::vec4& colour);

    void RenderLine(
        const glm::vec2& p1,
        const glm::vec2& p2,
        const glm::vec4& colour);

    void SetDebugRendering(bool val) { debug_enabled = val; }
    void SetVSync(bool val) { SDL_SetRenderVSync(handle.get(), val ? 1 : 0); }

    bool IsDebugRendering() const { return debug_enabled; }

private:
    bool debug_enabled = false;
    utility::unique_ptr<SDL_Renderer, SDL_DestroyRenderer> handle {};
};