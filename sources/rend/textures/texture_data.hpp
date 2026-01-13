#pragma once

#include <cstdint>
#include <functional>

namespace ars_graphics
{
class TextureData final
{
    TextureData& operator=(const TextureData&) = delete;
    TextureData(const TextureData&)            = delete;

public:
    void setData(void* data,
                 size_t data_size,
                 int width,
                 int height,
                 int channels,
                 std::function<void(void*)> deleter);

    size_t getChannels() const;

    int getWidth() const;
    int getHeight() const;

    size_t dataSize() const;

    const void* data() const;

    TextureData() = default;

    TextureData& operator=(TextureData&&) noexcept;
    TextureData(TextureData&&) noexcept;

    ~TextureData();

private:
    void clean();

    void nullData();

    std::function<void(void*)> m_deleter = nullptr;

    int m_width    = 0;
    int m_height   = 0;
    int m_channels = 0;

    size_t m_data_size = 0;

    void* m_data = nullptr;
};
} // namespace ars_graphics
