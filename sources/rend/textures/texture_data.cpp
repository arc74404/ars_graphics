#include "texture_data.hpp"

namespace ars_graphics
{

TextureData&
TextureData::operator=(TextureData&& other) noexcept
{
    if (this != &other)
    {
        clean();
        setData(other.m_data, other.m_data_size, other.m_width, other.m_height,
                other.m_channels, other.m_deleter);
        other.nullData();
    }
    return *this;
}

void
TextureData::nullData()
{
    m_data      = nullptr;
    m_data_size = 0;
    m_width     = 0;
    m_height    = 0;
    m_channels  = 0;
    m_deleter   = nullptr;
}

TextureData::TextureData(TextureData&& other) noexcept
{
    *this = std::move(other);
}

void
TextureData::clean()
{
    if (m_deleter && m_data)
    {
        m_deleter(m_data);
    }
    nullData();
}

void
TextureData::setData(void* data,
                     size_t data_size,
                     int width,
                     int height,
                     int channels,
                     std::function<void(void*)> deleter)
{
    clean();
    m_data      = data;
    m_data_size = data_size;
    m_width     = width;
    m_height    = height;
    m_channels  = channels;
    m_deleter   = std::move(deleter);
}

size_t
TextureData::dataSize() const
{
    return m_data_size;
}

const void*
TextureData::data() const
{
    return m_data;
}

size_t
TextureData::getChannels() const
{
    return m_channels;
}

int
TextureData::getWidth() const
{
    return m_width;
}
int
TextureData::getHeight() const
{
    return m_height;
}

TextureData::~TextureData()
{
    clean();
}

} // namespace ars_graphics
