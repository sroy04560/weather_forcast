# CMakeLists.txt
include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.0
)

FetchContent_MakeAvailable(nlohmann_json)

# In your target, link against the library
add_executable(wether_forcast src/wether_api.cpp)
target_link_libraries(wether_forcast  PRIVATE nlohmann_json::nlohmann_json)