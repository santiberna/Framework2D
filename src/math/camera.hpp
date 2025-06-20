#pragma once
#include <math/types.hpp>
#include <SDL3/SDL_rect.h>

class FrameCamera
{
public:
    FrameCamera(const glm::mat3& transform)
        : inverse(glm::inverse(transform))
        , transform(transform)
    {
    }

    glm::vec2 ToScreenPoint(const glm::vec2& point) const;
    SDL_FRect ToScreenRect(const SDL_FRect& world_rect) const;

    glm::vec2 ToWorld(const glm::vec2& screen) const;

private:
    glm::mat3 inverse {};
    glm::mat3 transform {};
};

class PersistentCamera
{
public:
    FrameCamera MakeFrameCamera() const;

    glm::vec2 translation {};
    glm::vec2 resolution {};
    float zoom = 1.0f;
};
