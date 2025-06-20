#include <resources/animation.hpp>

size_t AnimationState::GetNextFrameRect(const Animation& active_anim, DeltaMS delta)
{
    if (active_anim.frames.empty())
    {
        return 0;
    }

    accum += delta;
    DeltaMS current_delay = active_anim.frames.at(current_frame).next_delay;

    while (accum > current_delay)
    {
        accum -= current_delay;

        current_frame = (current_frame + 1) % active_anim.frames.size();
        current_delay = active_anim.frames.at(current_frame).next_delay;
    }

    return active_anim.frames.at(current_frame).rect_index;
}