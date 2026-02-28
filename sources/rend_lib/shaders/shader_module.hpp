#pragma once
#include <fstream>
#include <string_view>

#include "../device/logical_device.hpp"

namespace ars_graphics
{

std::optional<vk::UniqueShaderModule>
createShaderModule(vk::Device device, std::ifstream& file);

} // namespace ars_graphics
