cmake_minimum_required(VERSION 3.21.3)

# ---- Общие настройки ----
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Пути к зависимостям ----
set(CMAKE_PREFIX_PATH "C:/Users/User/source/repos/utils_lib/install;${CMAKE_PREFIX_PATH}")
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")



# ==================================================
# 1. БИБЛИОТЕКА REND_LIB
# ==================================================
project(rend_lib)

# ---- Поиск внешних библиотек ----
find_package(glfw3 CONFIG REQUIRED)
find_path(TINYGLTF_INCLUDE_DIRS "tiny_gltf.h")
find_package(Vulkan REQUIRED)

# Собираем все исходники rend_lib
file(GLOB_RECURSE REND_LIB_SOURCES
    "sources/rend_lib/buffers/*.cpp"
    "sources/rend_lib/camera/*.cpp"
    "sources/rend_lib/cmdbuf/*.cpp"
    "sources/rend_lib/descriptors/*.cpp"
    "sources/rend_lib/device/*.cpp"
    "sources/rend_lib/images/*.cpp"
    "sources/rend_lib/matrial/*.cpp"
    "sources/rend_lib/render/*.cpp"
)

# Создаем статическую библиотеку
add_library(rend_lib STATIC ${REND_LIB_SOURCES})

# Указываем, где искать заголовочные файлы для самой библиотеки
target_include_directories(rend_lib PUBLIC 
    "sources/rend_lib"  # Сама папка библиотеки
    "sources/rend_lib/buffers"
    "sources/rend_lib/camera"
    "sources/rend_lib/cmdbuf"
    "sources/rend_lib/descriptors"
    "sources/rend_lib/device"
    "sources/rend_lib/images"
    "sources/rend_lib/matrial"
    "sources/rend_lib/render"
)

# Линкуем зависимости к библиотеке
target_link_libraries(rend_lib PUBLIC
    Vulkan::Vulkan
    glfw
)

target_include_directories(rend_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/rend_lib  # <-- вот это обеспечит #include <ars_render>
)

# Если tinygltf используется в заголовках rend_lib
target_include_directories(rend_lib PUBLIC ${TINYGLTF_INCLUDE_DIRS})

# ==================================================
# 2. ИСПОЛНЯЕМЫЙ ФАЙЛ USER_PART
# ==================================================
project(user_part)

# Ищем main.cpp и другие файлы пользовательской части
file(GLOB_RECURSE USER_SOURCES
    "sources/user_part/main/*.cpp"
    # Раскомментируй, если есть gui:
    # "sources/main/*.cpp"
)

# Создаем исполняемый файл
add_executable(user_part ${USER_SOURCES})

# Линкуем нашу библиотеку к исполняемому файлу
target_link_libraries(user_part PRIVATE
    rend_lib  # Вот тут просто подключаем библиотеку
)

# Если user_part использует glfw напрямую (например, создает окно)
target_link_libraries(user_part PRIVATE glfw)