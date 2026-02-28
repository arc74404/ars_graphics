#pragma once
#include "buffer.hpp"

namespace ars_graphics
{
class StorageBuffer final : public Buffer
{
public:
    StorageBuffer(const LogicalDevice& logical_device,
                  const PhysicalDevice& physical_device);

private:
};
} // namespace ars_graphics
