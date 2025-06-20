#pragma once
#include <memory>

namespace utility
{

template <typename T, auto Dx>
struct deleter
{
    void operator()(T* v) const
    {
        Dx(v);
    }
};

template <typename T, auto Dx>
using unique_ptr = std::unique_ptr<T, deleter<T, Dx>>;

}