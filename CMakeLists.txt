cmake_minimum_required(VERSION 3.21.3)

# if(MSVC)
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
#     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
# endif()


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")

project(vulkan_app)

find_package(glfw3 CONFIG REQUIRED)
find_path(TINYGLTF_INCLUDE_DIRS "tiny_gltf.h")
find_package(Vulkan REQUIRED)


file(GLOB_RECURSE SOURCES
    "sources/*.cpp" 
)
add_executable(vulkan_app ${SOURCES})

target_link_libraries(vulkan_app PRIVATE glfw)
target_include_directories(vulkan_app PRIVATE tinygltf::tinygltf) 
target_link_libraries(vulkan_app PRIVATE Vulkan::Vulkan)
