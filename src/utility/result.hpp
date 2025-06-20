#pragma once
#include <expected.hpp>
#include <string>

struct Error
{
    std::string message {};
};

template <typename T>
using Result = tl::expected<T, Error>;
using Failed = tl::unexpected<Error>;