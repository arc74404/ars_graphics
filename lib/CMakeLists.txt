cmake_minimum_required(VERSION 3.15)
project(arsrender_lib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# vcpkg
set(VCPKG_ROOT "C:/vcpkg")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

# Исходные файлы
file(GLOB_RECURSE SOURCES 
    "sources/*.cpp"
)

# Только Vulkan (tinygltf - header-only, не нужен find_package)
find_package(Vulkan REQUIRED)

# НЕ ИСПОЛЬЗУЙТЕ find_package для tinygltf!
# Вместо этого просто добавьте путь к инклудам vcpkg

# Библиотека
add_library(arsrender_lib ${SOURCES})

# Инклуд директории
target_include_directories(arsrender_lib PUBLIC
    # Текущая директория
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    
    # Все инклуды vcpkg (включая tinygltf)
    "${VCPKG_ROOT}/installed/x64-windows/include"
    
    # Vulkan инклуды (на всякий случай)
    ${Vulkan_INCLUDE_DIRS}
)

# Линковка
target_link_libraries(arsrender_lib PRIVATE 
    Vulkan::Vulkan
    # GLFW из vcpkg
    "${VCPKG_ROOT}/installed/x64-windows/lib/glfw3dll.lib"
    # Или если glfw установлен через vcpkg как статическая библиотека:
    # "${VCPKG_ROOT}/installed/x64-windows/lib/glfw3.lib"
)
