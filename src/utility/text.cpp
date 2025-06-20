#include <utility/text.hpp>
#include <convert.hpp>
#include <charconv>

unicode::String unicode::FromASCII(const std::string& ascii)
{
    String out {};
    out.reserve(ascii.size());

    for (auto c : ascii)
    {
        out.push_back(static_cast<Codepoint>(c));
    }

    return out;
}

unicode::String unicode::FromUTF8(const std::string& utf8)
{
    auto copy = utf8;
    unicode::String out {};

    while (copy.size())
    {
        uint32_t utf32 {};
        auto bytes_consumed = c_utf8::capi::c_utf8_buf_to_utf32_char_b(&utf32, copy.data(), nullptr);
        out.push_back(utf32);

        copy.erase(copy.begin(), copy.begin() + bytes_consumed);
    }

    return out;
}

std::wstring unicode::AsWideString(const unicode::String& unicode)
{
    std::wstring out;
    for (auto c : unicode)
        out.push_back((wchar_t)(c));
    return out;
}

template <typename T>
std::optional<std::pair<T, ptrdiff_t>> ParseNumberWithOffset(std::string_view str)
{
    T val {};
    auto result = std::from_chars(str.data(), str.data() + str.size(), val);
    if (result.ec == std::errc {})
    {
        ptrdiff_t diff = result.ptr - str.data();
        return std::make_pair(val, diff);
    }
    else
    {
        return std::nullopt;
    }
}

template <typename T>
std::optional<T> parse::Number(std::string_view str)
{
    if (auto v = ParseNumberWithOffset<T>(str))
    {
        return v->first;
    }
    return std::nullopt;
}

template <typename T>
std::vector<T> parse::AllNumbers(std::string_view str)
{
    std::vector<T> out {};

    auto end_it = str.end();
    for (auto it = str.begin(); it != end_it; ++it)
    {
        if (std::isdigit(*it) || *it == '-')
        {
            std::string_view remaining_view = { it, end_it };
            if (auto parsed_num = ParseNumberWithOffset<T>(remaining_view))
            {
                it += parsed_num.value().second;
                out.emplace_back(parsed_num.value().first);
            }
        }

        if (it == end_it)
            break;
    }

    return out;
}

#define INSTANTIATE_PARSE_NUMBER(T) \
    template std::optional<T> parse::Number<T>(std::string_view str)

INSTANTIATE_PARSE_NUMBER(double);
INSTANTIATE_PARSE_NUMBER(float);
INSTANTIATE_PARSE_NUMBER(int16_t);
INSTANTIATE_PARSE_NUMBER(uint16_t);
INSTANTIATE_PARSE_NUMBER(int32_t);
INSTANTIATE_PARSE_NUMBER(uint32_t);
INSTANTIATE_PARSE_NUMBER(int64_t);
INSTANTIATE_PARSE_NUMBER(uint64_t);

#define INSTANTIATE_PARSE_ALL_NUMBERS(T) \
    template std::vector<T> parse::AllNumbers<T>(std::string_view str)

INSTANTIATE_PARSE_ALL_NUMBERS(double);
INSTANTIATE_PARSE_ALL_NUMBERS(float);
INSTANTIATE_PARSE_ALL_NUMBERS(int16_t);
INSTANTIATE_PARSE_ALL_NUMBERS(uint16_t);
INSTANTIATE_PARSE_ALL_NUMBERS(int32_t);
INSTANTIATE_PARSE_ALL_NUMBERS(uint32_t);
INSTANTIATE_PARSE_ALL_NUMBERS(int64_t);
INSTANTIATE_PARSE_ALL_NUMBERS(uint64_t);