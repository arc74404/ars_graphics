#pragma once

#include "buffer.hpp"

namespace ars_graphics
{
class UniformBuffer final : public Buffer
{
public:
    UniformBuffer(const LogicalDevice& logical_device,
                  const PhysicalDevice& physical_device);

private:
};
} // namespace ars_graphics
