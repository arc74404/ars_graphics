# Просто не используйте find_package для GLFW
# Указывайте пути напрямую:

set(CMAKE_CXX_STANDARD 20)
set(VCPKG_ROOT "C:/vcpkg")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

file(GLOB_RECURSE SOURCES "sources/*.cpp")

find_package(Vulkan REQUIRED)  # только Vulkan

add_library(arsrender_lib ${SOURCES})
target_include_directories(arsrender_lib PUBLIC 
    .
    "${VCPKG_ROOT}/installed/x64-windows/include"
)

target_link_libraries(arsrender_lib PRIVATE 
    Vulkan::Vulkan
    "${VCPKG_ROOT}/installed/x64-windows/lib/glfw3dll.lib"
)