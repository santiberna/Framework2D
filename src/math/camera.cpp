#include <math/camera.hpp>

glm::mat3 MakeViewMatrix(const glm::vec2& camera_position, float camera_zoom)
{
    glm::mat3 mat = glm::mat3(1.0f);
    mat[0] = glm::vec3(1 / camera_zoom, 0.0f, 0.0f);
    mat[1] = glm::vec3(0.0f, 1 / camera_zoom, 0.0f);
    mat[2] = glm::vec3(camera_position, 1.0f);
    return glm::inverse(mat);
}

glm::mat3 MakeCameraProjection(glm::vec2 window_size)
{
    float aspect_ratio = window_size.x / window_size.y;

    glm::mat3 mat = glm::mat3(1.0f);
    mat[0] = glm::vec3(1.0f, 0.0f, 0.0f);
    mat[1] = glm::vec3(0.0f, 1.0f, 0.0f);
    mat[2] = glm::vec3(window_size * 0.5f, 1.0f);

    return mat;
}

FrameCamera PersistentCamera::MakeFrameCamera() const
{
    auto view = MakeViewMatrix(translation, zoom);
    auto projection = MakeCameraProjection(resolution);
    auto combine = projection * view;

    return FrameCamera { combine };
}

glm::vec2 FrameCamera::ToScreenPoint(const glm::vec2& point) const
{
    return transform * glm::vec3(point, 1.0f);
}

SDL_FRect FrameCamera::ToScreenRect(const SDL_FRect& world_rect) const
{
    auto transformed = transform * glm::vec3(world_rect.x, world_rect.y, 1.0f);
    auto transformed_size = transform * glm::vec3(world_rect.w, world_rect.h, 0.0f);

    return SDL_FRect {
        transformed.x, transformed.y, transformed_size.x, transformed_size.y
    };
}

glm::vec2 FrameCamera::ToWorld(const glm::vec2& screen) const
{
    return inverse * glm::vec3(screen, 1.0f);
}