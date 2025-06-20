#include <engine/common.hpp>

#include <utility/log.hpp>
#include <SDL3/SDL_init.h>
#include <SDL3_mixer/SDL_mixer.h>

void SDL::AbortIfFailed(bool success)
{
    if (!success)
    {
        Log(SDL_GetError());
        std::abort();
    }
}

void SDL::Init()
{
    AbortIfFailed(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO));
    AbortIfFailed(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG));
    AbortIfFailed(Mix_OpenAudio(0, nullptr));
}

void SDL::Shutdown()
{
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}