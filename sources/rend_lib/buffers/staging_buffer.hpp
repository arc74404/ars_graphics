#pragma once

#include "buffer.hpp"

namespace ars_graphics
{
class StagingBuffer final : public Buffer
{
public:
    StagingBuffer(const LogicalDevice& logical_device,
                  const PhysicalDevice& physical_device);

private:
};
} // namespace ars_graphics
