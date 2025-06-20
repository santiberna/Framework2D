
#include <ui/core/menu.hpp>

void Menu::Draw(Renderer& renderer, glm::uvec2 window_size, const UICursorInfo& cursor_params)
{
    float min_size = std::min(window_size.x, window_size.y);

    UIDrawInfo initial {
        glm::vec2(window_size) * 0.5f,
        { min_size, min_size },
        colour::WHITE
    };

    for (auto it = elements.begin(); it != elements.end(); it = elements.next_sibling(it))
    {
        DrawElement(renderer, it, initial, cursor_params);
    }
}

void Menu::DrawElement(Renderer& renderer, NodeIterator element, const UIDrawInfo& parent_info, const UICursorInfo& cursor_params)
{
    auto& elem = **element;

    if (!elem.active)
        return;

    UIDrawInfo current_draw_info = parent_info * elem.local_transform;

    {
        elem.Draw(renderer, current_draw_info, cursor_params);

        if (renderer.IsDebugRendering())
        {
            renderer.RenderRect(
                current_draw_info.rect_center,
                current_draw_info.rect_size,
                current_draw_info.node_colour);
        }
    }

    for (auto it = elements.begin(element); it != elements.end(element); ++it)
    {
        // Log("Drawing Child Element");
        DrawElement(renderer, it, current_draw_info, cursor_params);
    }
}

Menu::NodeIterator Menu::AddChildNode(Menu::NodeIterator parent, std::unique_ptr<UINode>&& node)
{
    return elements.append_child(parent, std::move(node));
}

Menu::NodeIterator Menu::AddRootNode(std::unique_ptr<UINode>&& node)
{
    return elements.insert(elements.begin(), std::move(node));
}