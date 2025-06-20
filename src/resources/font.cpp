#include <resources/font.hpp>
#include <utility/file_io.hpp>
#include <stb_truetype.h>
#include <math/rect_pack.hpp>

#include <tracy/Tracy.hpp>

std::optional<Font> Font::FromFile(Renderer& renderer, const std::string& font_file, const FontLoadInfo& load_info)
{
    ZoneScopedN("Font Loading");

    std::vector<std::byte> font_data {};
    stbtt_fontinfo font {};

    // Load font file
    {
        auto file_stream = fileIO::OpenReadStream(font_file);
        if (!file_stream)
            return std::nullopt;

        font_data = fileIO::DumpStreamIntoBytes(file_stream.value());

        if (!stbtt_InitFont(&font, std::bit_cast<const uint8_t*>(font_data.data()), 0))
        {
            return std::nullopt;
        }
    }

    FontMetrics metrics {};
    float font_scale = stbtt_ScaleForPixelHeight(&font, load_info.resolutionY);

    // Get general metrics
    {
        int line_gap, ascent, descent;
        stbtt_GetFontVMetrics(&font, &ascent, &descent, &line_gap);

        metrics.resolution = load_info.resolutionY;
        metrics.ascent = (float)ascent * font_scale;
        metrics.descent = (float)descent * font_scale;
        metrics.line_gap = (float)line_gap * font_scale;
    }

    // Find all codepoints
    std::vector<unicode::Codepoint> codepoints_found {};

    for (const auto& range : load_info.codepoint_ranges)
    {
        for (uint32_t i = range.first; i < range.second; i++)
        {
            if (stbtt_FindGlyphIndex(&font, i))
            {
                codepoints_found.emplace_back(i);
            }
        }
    }

    // Collect all kerning data
    std::unordered_map<unicode::CodepointPair, float> kerning_table {};

    for (size_t i = 0; i < codepoints_found.size(); i++)
    {
        for (size_t j = 0; j < codepoints_found.size(); j++)
        {
            auto point1 = codepoints_found[i];
            auto point2 = codepoints_found[j];

            int kerning = stbtt_GetGlyphKernAdvance(&font, stbtt_FindGlyphIndex(&font, point1), stbtt_FindGlyphIndex(&font, point2));

            if (kerning != 0)
            {
                kerning_table.emplace(std::make_pair(point1, point2), (float)kerning * font_scale);
            }
        }
    }

    // Collect codepoint metrics

    std::unordered_map<unicode::Codepoint, CodepointInfo> codepoint_info {};

    for (auto codepoint : codepoints_found)
    {
        int x1, x2, y1, y2;
        stbtt_GetCodepointBitmapBox(&font, codepoint, font_scale, font_scale, &x1, &y1, &x2, &y2);

        int left_bearing, advance;
        stbtt_GetCodepointHMetrics(&font, codepoint, &advance, &left_bearing);

        CodepointInfo info {};
        info.advance = (float)advance * font_scale;
        info.left_bearing = (float)left_bearing * font_scale;
        info.offset = glm::vec2(x1, y1);
        info.size = glm::vec2(x2 - x1, y2 - y1);

        codepoint_info.emplace(codepoint, info);
    }

    // Pack all codepoint rectangles

    math::PackResult packed_codepoints {};

    {
        std::vector<glm::uvec2> codepoint_rects {};
        for (auto codepoint : codepoints_found)
        {
            codepoint_rects.emplace_back(glm::uvec2(codepoint_info[codepoint].size));
        }

        packed_codepoints = math::PackRectanglesUntilSuccess(
            codepoint_rects,
            load_info.initial_atlas_packing_area,
            load_info.atlas_margin);
    }

    // Render into Atlas

    Image font_atlas {};

    if (auto new_bitmap = Image::New(packed_codepoints.packing_area.x, packed_codepoints.packing_area.y))
    {
        font_atlas = std::move(new_bitmap.value());
    }

    for (size_t i = 0; i < codepoints_found.size(); i++)
    {
        auto rect = packed_codepoints.packed_rects.at(i);

        if (rect.h == 0 || rect.w == 0)
            continue;

        auto codepoint = codepoints_found[i];
        codepoint_info[codepoint].atlas_index = i;

        int width, height, x_off, y_off;
        uint8_t* render = stbtt_GetCodepointBitmap(&font, font_scale, font_scale, codepoint, &width, &height, &x_off, &y_off);

        if (render)
        {
            for (int j = 0; j < height; j++)
            {
                for (int i = 0; i < width; i++)
                {
                    size_t char_index = j * width + i;
                    uint8_t alpha_val = render[char_index];

                    uint32_t pixel_val = SDL_MapSurfaceRGBA(font_atlas.surface.get(), 0xFF, 0xFF, 0xFF, alpha_val);

                    font_atlas.data.At(i + rect.x, j + rect.y) = pixel_val;
                }
            }
        }

        stbtt_FreeBitmap(render, nullptr);
    }

    Font out {};

    if (auto texture = Texture::SharedFromImage(renderer, font_atlas))
    {
        out.font_atlas = TextureAtlas::FromTextureAndRects(texture, std::move(packed_codepoints.packed_rects));
    }
    else
    {
        return std::nullopt;
    }

    out.codepoint_data = std::move(codepoint_info);
    out.kerning_table = std::move(kerning_table);
    out.font_metrics = metrics;

    return out;
}

CodepointInfo Font::GetCodepointInfo(unicode::Codepoint codepoint) const
{
    if (auto it = codepoint_data.find(codepoint); it != codepoint_data.end())
    {
        return it->second;
    }

    return codepoint_data.find(unicode::MISSING_CODEPOINT)->second;
}

float Font::GetKerning(unicode::Codepoint first, unicode::Codepoint next) const
{
    if (auto it = kerning_table.find({ first, next }); it != kerning_table.end())
    {
        return it->second;
    }
    return 0.0f;
}

// Layout text helpers

float CalcKerning(const Font& font, const unicode::String& text, size_t index)
{
    float out {};
    if (index == text.size() - 1)
    {
        out = 0.0f;
    }
    else
    {
        out = font.GetKerning(text[index], text[index + 1]);
    }

    return out;
}

std::vector<unicode::String> SplitIntoWords(const unicode::String& text)
{
    std::vector<unicode::String> out {};

    for (auto c : text)
    {
        if (c == unicode::SPACE_CODEPOINT)
        {
            out.emplace_back();
            out.back().push_back(unicode::SPACE_CODEPOINT);
        }
        else if (c == unicode::LINEBREAK_CODEPOINT)
        {
            out.emplace_back();
            out.back().push_back(c);
        }
        else
        {
            if (out.empty() || out.back().front() == unicode::SPACE_CODEPOINT || out.back().front() == unicode::LINEBREAK_CODEPOINT)
                out.emplace_back();

            out.back().push_back(c);
        }
    }

    return out;
}

std::vector<CodepointDraw> CodepointDraw::LayoutText(const Font& font, const unicode::String& text, const CodepointDraw::LayoutParams& params)
{
    std::vector<CodepointDraw> out {};

    auto font_metrics = font.GetFontMetrics();
    float line_offset = (font_metrics.line_gap - font_metrics.descent + font_metrics.ascent) * params.scale;

    if (text.empty())
    {
        glm::vec2 none {};

        if (params.center_horizontally)
        {
            none.x = params.rect.x * 0.5f;
        }
        if (params.center_vertically)
        {
            none.y = params.rect.y * 0.5f - line_offset * 0.5f;
        }

        out.emplace_back(none);
        return out;
    }

    if (text.empty() == false)
    {
        auto words = SplitIntoWords(text);

        // First pass, organize into lines

        struct Line
        {
            unicode::String text;
            float length {};
        };

        std::vector<Line> lines {};
        lines.emplace_back();

        float line_size = 0.0f;
        for (auto& word : words)
        {
            if (word.front() == unicode::LINEBREAK_CODEPOINT)
            {
                lines.emplace_back();
                line_size = 0.0f;
                continue;
            }

            float word_length {};

            for (size_t i = 0; i < word.length(); i++)
            {
                auto glyph = font.GetCodepointInfo(word[i]);
                float kerning = CalcKerning(font, word, i);
                word_length += (glyph.advance + kerning) * params.scale;
            }

            if (line_size + word_length > params.rect.x)
            {
                lines.emplace_back();
                line_size = 0.0f;
            }

            line_size += word_length;
            lines.back().text += word;
            lines.back().length = line_size;
        }

        // Second pass, draw stuff

        float vertical_center = (params.rect.y - (line_offset * lines.size())) * 0.5f;

        glm::vec2 pen_position = { 0.0f, -line_offset };

        if (params.center_vertically)
        {
            pen_position.y += vertical_center;
        }

        for (auto& line : lines)
        {
            float horizontal_center = (params.rect.x - line.length) * 0.5f;

            pen_position.y += line_offset;
            pen_position.x = 0.0f;

            if (params.center_horizontally)
            {
                pen_position.x += horizontal_center;
            }

            for (size_t i = 0; i < line.text.size(); i++)
            {
                auto glyph = font.GetCodepointInfo(line.text[i]);
                float kerning = CalcKerning(font, line.text, i);

                glm::vec2 glyph_draw_offset = glm::vec2 {
                    glyph.left_bearing,
                    (glyph.offset.y + font_metrics.ascent)
                } * params.scale;

                out.emplace_back(pen_position + glyph_draw_offset, glyph.atlas_index);
                pen_position.x += (glyph.advance + kerning) * params.scale;
            }
        }

        out.emplace_back(pen_position);
    }

    return out;
}