# MAIN PROJECT

add_library(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC ThirdParty)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(${PROJECT_NAME} 
    PUBLIC cxx_std_20)

file(GLOB_RECURSE source CONFIGURE_DEPENDS
    "engine/*.cpp" "engine/*.hpp"
    "ui/*.cpp" "ui/*.hpp"
    "math/*.cpp" "math*.hpp"
    "resources/*.cpp" "resources/*.hpp"
    "utility/*.cpp" "utility/*.hpp"
    "thread/*.cpp" "thread/*.hpp"
)

target_sources(${PROJECT_NAME} PRIVATE ${source})
