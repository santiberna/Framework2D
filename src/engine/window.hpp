#pragma once
#include <engine/common.hpp>
#include <SDL3/SDL_video.h>
#include <math/types.hpp>
#include <utility/memory.hpp>
#include <engine/renderer.hpp>
#include <engine/input.hpp>

class Window
{
public:
    Window(const char* title, glm::uvec2 size);

    SDL_Window* GetWindow() const;
    Renderer& GetRenderer() const;
    InputEventSystem& GetInput() const;

    SDL_WindowID GetID() const;

    glm::uvec2 GetSize() const;
    void SetSize(glm::uvec2 size);

    void ProcessEvents();
    void RenderPresent();

private:
    utility::unique_ptr<SDL_Window, SDL_DestroyWindow> window;
    std::unique_ptr<InputEventSystem> input;
    std::unique_ptr<Renderer> renderer;
};