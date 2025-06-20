#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace math
{
inline float VectorAngle(const glm::vec2& from, const glm::vec2& target)
{
    // Rotation from two unit vectors
    float cos = from.x * target.x + from.y * target.y;
    float sin = from.x * target.y + from.y * target.x;

    return std::atan2(sin, cos);
}

inline glm::vec2 AngleToVector(float angle)
{
    return { std::cosf(angle), std::sinf(angle) };
}

inline float AngleWrap(float angle)
{
    return std::fmodf(angle, 2.0f * glm::pi<float>());
}
}