#pragma once
#include <utility/time.hpp>

struct Keyframe
{
    size_t rect_index {};
    DeltaMS next_delay {};
};

struct Animation
{
    std::string name {};
    std::vector<Keyframe> frames {};
};

struct AnimationState
{
    size_t current_frame = 0;
    DeltaMS accum {};

    size_t GetNextFrameRect(const Animation& active_anim, DeltaMS delta);
};