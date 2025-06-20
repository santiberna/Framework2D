#include <engine/window.hpp>

Window::Window(const char* title, glm::uvec2 size)
{
    SDL_Window* window_ptr = nullptr;
    SDL_Renderer* renderer_ptr = nullptr;

    SDL_CreateWindowAndRenderer(title, size.x, size.y, 0, &window_ptr, &renderer_ptr);

    window = utility::unique_ptr<SDL_Window, SDL_DestroyWindow>(window_ptr);
    renderer = std::make_unique<Renderer>(renderer_ptr);
    input = std::make_unique<InputEventSystem>();
}

SDL_WindowID Window::GetID() const
{
    return SDL_GetWindowID(window.get());
}

Renderer& Window::GetRenderer() const
{
    return *renderer;
}

SDL_Window* Window::GetWindow() const
{
    return window.get();
}

InputEventSystem& Window::GetInput() const
{
    return *input;
}

glm::uvec2 Window::GetSize() const
{
    int width = 0, height = 0;
    SDL_GetWindowSize(window.get(), &width, &height);
    return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
}

void Window::SetSize(glm::uvec2 size)
{
    SDL_SetWindowSize(window.get(), size.x, size.y);
}

void Window::ProcessEvents()
{
    SDL_Event event {};
    while (SDL_PollEvent(&event))
    {
        GetInput().ProcessEvent(GetWindow(), event);
    }
}

void Window::RenderPresent()
{
    SDL::AbortIfFailed(SDL_RenderPresent(renderer->GetRenderer()));
}
