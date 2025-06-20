#include <ui/widgets/checkbox.hpp>
#include <math/aabb.hpp>

void CheckBox::Draw([[maybe_unused]] Renderer& renderer, const UIDrawInfo& draw_info, const UICursorInfo& cursor_info)
{
    bool cursor_click = cursor_info.cursor_state == InputState::RELEASED && CheckAABB(draw_info.rect_center, draw_info.rect_size, cursor_info.cursor_position);

    if (cursor_click)
    {
        state = !state;
        on_value_set(*this, state);
    }
}
