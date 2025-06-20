#include <engine/input.hpp>
#include <SDL3/SDL_clipboard.h>

void InputEventSystem::ProcessEvent(SDL_Window* window, const SDL_Event& ev)
{
    switch (ev.type)
    {
    case SDL_EVENT_KEY_DOWN:
    {
        if (!ev.key.repeat)
        {
            on_key_press[ev.key.key](true);
        }

        if (SDL_TextInputActive(window))
        {
            if (ev.key.key == SDLK_BACKSPACE)
            {
                on_text_input({ unicode::BACKSPACE_CODEPOINT });
            }
            if (ev.key.key == SDLK_V && ev.key.mod & SDL_KMOD_CTRL)
            {
                char* data = SDL_GetClipboardText();
                on_text_input(unicode::FromUTF8({ data }));
                SDL_free(data);
            }
            if (ev.key.key == SDLK_RETURN)
            {
                on_text_input({ unicode::LINEBREAK_CODEPOINT });
            }
        }

        break;
    }
    case SDL_EVENT_KEY_UP:
        on_key_press[ev.key.key](false);
        break;
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        on_close();
        break;
    case SDL_EVENT_MOUSE_MOTION:
    {
        glm::vec2 mouse_pos {};
        SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
        on_mouse_movement(mouse_pos);
        break;
    }
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    {
        on_button_click[ev.button.button](true);
        break;
    }
    case SDL_EVENT_MOUSE_BUTTON_UP:
    {
        on_button_click[ev.button.button](false);
        break;
    }
    case SDL_EVENT_MOUSE_WHEEL:
    {
        on_mouse_wheel({ ev.wheel.x, ev.wheel.y });
        break;
    }
    case SDL_EVENT_TEXT_INPUT:
    {
        auto string = unicode::FromUTF8(ev.text.text);
        on_text_input(string);
        break;
    }
    default:
        break;
    }
}
