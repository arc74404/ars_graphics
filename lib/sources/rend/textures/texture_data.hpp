#pragma once

#include <cstdint>

namespace ars_graphics
{
class TextureData
{
public:
    void setData(const uint8_t* data,
                 size_t data_size,
                 int width,
                 int height,
                 int channels);

private:
    int m_width;
    int m_height;
    int m_channels;

    size_t m_data_size;

    const uint8_t* m_data;
};
} // namespace ars_graphics
