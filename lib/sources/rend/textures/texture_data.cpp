#include "texture_data.hpp"

namespace ars_graphics
{
void
TextureData::setData(const uint8_t* data,
                     size_t data_size,
                     int width,
                     int height,
                     int channels)
{
    m_data      = data;
    m_data_size = data_size;
    m_width     = width;
    m_height    = height;
    m_channels  = channels;
}
} // namespace ars_graphics
