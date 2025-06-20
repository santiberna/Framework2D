#include <ui/widgets/text_box.hpp>
#include <utility/log.hpp>

void TextBox::Draw(Renderer& renderer, const UIDrawInfo& draw_params, const UICursorInfo& cursor_params)
{
    if (font == nullptr)
    {
        Log("[UI Warn] No font assigned to textbox");
        return;
    }

    auto global_offset = draw_params.rect_center - draw_params.rect_size * 0.5f;

    CodepointDraw::LayoutParams params {};

    params.scale = font_size;
    params.rect = draw_params.rect_size;
    params.center_horizontally = center.x;
    params.center_vertically = center.y;

    cached_layout = CodepointDraw::LayoutText(*font, text, params);

    for (size_t i = 0; i < cached_layout.size() - 1; i++)
    {
        auto& c = cached_layout[i];

        auto src_rect = font->GetAtlas().GetSpriteFRect(c.atlas_index).value();
        auto draw_off = global_offset + c.offset;

        SDL_FRect dst_rect {};
        dst_rect.x = draw_off.x;
        dst_rect.y = draw_off.y;
        dst_rect.h = src_rect.h * font_size;
        dst_rect.w = src_rect.w * font_size;

        renderer.RenderTextureRect(font->GetAtlas().GetTexture(), dst_rect, &src_rect, draw_params.node_colour);

        if (renderer.IsDebugRendering())
        {
            glm::vec2 center = glm::vec2(dst_rect.x, dst_rect.y) + glm::vec2(dst_rect.w, dst_rect.h) * 0.5f;
            renderer.RenderRect(center, { dst_rect.w, dst_rect.h }, draw_params.node_colour);
        }
    }
}