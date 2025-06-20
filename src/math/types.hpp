#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/mat3x3.hpp>

namespace glm
{
template <typename A>
void serialize(A& ar, vec2& self)
{
    ar(self.x);
    ar(self.y);
}

}

namespace std
{
template <>
struct hash<glm::uvec2>
{
    std::size_t operator()(const glm::uvec2& p) const
    {
        std::size_t h1 = std::hash<uint32_t> {}(p.x);
        std::size_t h2 = std::hash<uint32_t> {}(p.y);
        return h1 ^ (h2 << 1);
    }
};

template <>
struct hash<glm::ivec2>
{
    std::size_t operator()(const glm::uvec2& p) const
    {
        std::size_t h1 = std::hash<int32_t> {}(p.x);
        std::size_t h2 = std::hash<int32_t> {}(p.y);
        return h1 ^ (h2 << 1);
    }
};
}

namespace math
{

constexpr glm::vec2 UP = { 0.0f, -1.0f };
constexpr glm::vec2 RIGHT = { 1.0f, 0.0f };

struct Transform2D
{
    glm::vec2 translation {};
    glm::vec2 scale { 1.0f, 1.0f };
    float rotation {};
};

template <typename T>
T Lerp(float t, const T& a, const T& b)
{
    return a * (1.0f - t) + b * (t);
}

}