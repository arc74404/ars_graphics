#include "shader_module.hpp"

#include <fstream>
namespace
{
std::vector<char>
readFile(std::ifstream& file)
{
    file.seekg(0, std::ios::end);
    size_t filesize = file.tellg();

    std::vector<char> buffer(filesize);
    file.seekg(0);
    file.read(buffer.data(), filesize);

    file.close();

    return buffer;
}
} // namespace

namespace ars_graphics
{

const vk::ShaderModule&
ShaderModule::get() const
{
    return m_shader_module.get();
}

ShaderModule::ShaderModule(const LogicalDevice& device, std::ifstream& file)
{
    std::vector<char>&& source_code = readFile(file);

    vk::ShaderModuleCreateInfo module_info = {};
    module_info.flags                      = vk::ShaderModuleCreateFlags();
    module_info.codeSize                   = source_code.size();
    module_info.pCode = reinterpret_cast<const uint32_t*>(source_code.data());

    auto&& res = device.get().createShaderModuleUnique(module_info);

    if (res.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed create shader module");
    }
    m_shader_module = std::move(res.value);
}

} // namespace ars_graphics
