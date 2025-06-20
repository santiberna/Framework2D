#pragma once
#include <vector>

template <typename T>
class Array2D
{
public:
    Array2D() = default;

    Array2D(std::vector<T>&& data, size_t width, size_t height)
        : data(std::move(data))
        , width(width)
        , height(height)
    {
    }

    Array2D(size_t width, size_t height)
        : width(width)
        , height(height)
    {
        data = std::vector<T>(width * height);
    }

    Array2D(size_t width, size_t height, const T& init)
        : width(width)
        , height(height)
    {
        data = std::vector<T>(width * height, init);
    }

    size_t GetWidth() const { return width; }
    size_t GetHeight() const { return height; }

    T& At(size_t x, size_t y) { return data[x + y * width]; }
    const T& At(size_t x, size_t y) const { return data[x + y * width]; }

    struct Iterator2D
    {
        Array2D<T>* owner {};
        size_t x {};
        size_t y {};

        void operator++()
        {
            ++x;
            if (x >= owner->width)
            {
                x = 0;
                ++y;
            }
        }
        void operator++(int)
        {
            ++x;
            if (x >= owner->width)
            {
                x = 0;
                ++y;
            }
        }
        bool operator!=(const Iterator2D& other) const { return x != other.x || y != other.y; }
        T& operator*() { return owner->At(x, y); }
        const T& operator*() const { return owner->At(x, y); }
    };

    T* Data() { return data.data(); }
    Iterator2D begin() { return { this, 0, 0 }; }
    Iterator2D end() { return { this, 0, height }; }

private:
    std::vector<T> data;
    size_t width {};
    size_t height {};
};