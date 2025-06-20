#pragma once

#include <glm/glm.hpp>
#include <utility/text.hpp>
#include <engine/renderer.hpp>
#include <resources/texture_atlas.hpp>
#include <unordered_map>

struct FontMetrics
{
    float resolution {}, ascent {}, descent {}, line_gap {};
};

struct CodepointInfo
{
    glm::vec2 offset {};
    glm::vec2 size {};
    uint32_t atlas_index {};
    float advance {};
    float left_bearing {};
};

struct FontLoadInfo
{
    std::vector<unicode::CodepointPair> codepoint_ranges {};
    glm::uvec2 initial_atlas_packing_area { 128, 128 };
    float resolutionY = 32.0f;
    uint32_t atlas_margin = 1;
};

class Font
{
public:
    static std::optional<Font> FromFile(Renderer& renderer, const std::string& font_file, const FontLoadInfo& load_params);

    static std::shared_ptr<Font> SharedFromFile(Renderer& renderer, const std::string& font_file, const FontLoadInfo& load_params)
    {
        if (auto font = FromFile(renderer, font_file, load_params))
            return std::make_shared<Font>(std::move(font.value()));
        return nullptr;
    }

    FontMetrics GetFontMetrics() const { return font_metrics; }

    const TextureAtlas& GetAtlas() const
    {
        return font_atlas;
    }

    CodepointInfo GetCodepointInfo(unicode::Codepoint codepoint) const;
    float GetKerning(unicode::Codepoint first, unicode::Codepoint next) const;

private:
    // Font metrics
    FontMetrics font_metrics {};
    std::unordered_map<unicode::Codepoint, CodepointInfo> codepoint_data {};
    std::unordered_map<unicode::CodepointPair, float> kerning_table {};

    // Atlas
    TextureAtlas font_atlas {};
};

struct CodepointDraw
{
    glm::vec2 offset {};
    uint32_t atlas_index {};

    struct LayoutParams
    {
        glm::vec2 rect {};
        float scale = 1.0f;
        bool center_horizontally = false;
        bool center_vertically = false;
    };

    static std::vector<CodepointDraw> LayoutText(const Font& font, const unicode::String& text, const LayoutParams& params);
};