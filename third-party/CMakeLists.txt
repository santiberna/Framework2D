add_library(ThirdParty STATIC)

# SDL

FetchContent_Declare(
    SDL_lib
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG main
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(SDL_lib)

# SDL_Mixer

FetchContent_Declare(
    SDL_mixer_lib
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG main 
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(SDL_mixer_lib)

# GLM

FetchContent_Declare(
    GLM_lib
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(GLM_lib)

# MagicEnum

FetchContent_Declare(
    MagicEnum
    GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
    GIT_TAG v0.9.6
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(MagicEnum)

# Tracy Profiler

FetchContent_Declare(
    Tracy
    GIT_REPOSITORY https://github.com/wolfpld/tracy.git
    GIT_TAG v0.11.1
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(Tracy)

# Cereal

FetchContent_Declare(
    Cereal
    GIT_REPOSITORY https://github.com/USCiLab/cereal.git
    GIT_TAG v1.3.2
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

set(BUILD_DOC OFF)
set(BUILD_SANDBOX OFF)
set(SKIP_PERFORMANCE_COMPARISON ON)

FetchContent_MakeAvailable(Cereal)

# LINK DEPENDENCIES

target_compile_features(ThirdParty
    PUBLIC cxx_std_17)

target_include_directories(ThirdParty
    PUBLIC "other"
    PUBLIC "stb"
    PUBLIC "signals"
)

target_sources(ThirdParty
    PRIVATE "signals/signals.cpp"
    PRIVATE "stb/stb.cpp"
)

target_link_libraries(ThirdParty 
    PUBLIC SDL3::SDL3
    PUBLIC SDL3_mixer::SDL3_mixer 
    PUBLIC glm::glm
    PUBLIC cereal::cereal
    PUBLIC Tracy::TracyClient
    PUBLIC magic_enum::magic_enum
    )