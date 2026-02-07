#pragma once

#include <initializer_list>
#include <vector>

#include "buffer.hpp"

namespace ars_graphics
{
class IndexBuffer final : public Buffer
{
public:
    IndexBuffer();

    void bind( vk::CommandBuffer command_buffer) const;

    void draw(vk::CommandBuffer command_buffer,
              uint32_t index_count,
              uint32_t instance_count,
              uint32_t first_index,
              uint32_t vertex_offset,
              uint32_t first_instance) const;

private:
};
} // namespace ars_graphics
