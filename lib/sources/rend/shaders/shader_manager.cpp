#include "shader_manager.hpp"

#include <exception>
#include <fstream>
#include <iostream>

#include "../device/logical_device.hpp"

namespace ars_graphics
{

ShaderManager::ShaderManager(const LogicalDevice& device,
                             const std::string& shader_folder_path)
{
    const std::unordered_map<ShaderType, std::string> name_type_connections = {
        {ShaderType::DEFAULT_2D_VERTEX, "default_2d_vertex_shader.vert.spv"},
        {ShaderType::DEFAULT_3D_VERTEX, "default_3d_vertex_shader.vert.spv"},
        {ShaderType::DEFAULT_FRAGMENT,  "default_fragment_shader.frag.spv" }
    };

    for (auto&& con : name_type_connections)
    {
        std::string filepath = shader_folder_path + '/' + con.second;
        std::fstream file{filepath, std::ios_base::binary, std::ios_base::app};

        if (false == file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + filepath);
        }
        ShaderModule new_module(device, file);

        m_modules.emplace(con.first, std::move(new_module));
    }
}

const ShaderModule&
ShaderManager::operator[](ShaderType st) const
{
    return m_modules.at(st);
}

// void
// ShaderManager::setup(bool need_compile)
// {
// }

// std::string_view
// ShaderManager::cutSPV(const std::string& str) const
// {
//     std::string_view res(str);
//     return res.substr(0, res.size() - 4);
// }

// void
// ShaderManager::compile(
//     const std::string& ds_folder_path,
//     const std::unordered_map<ShaderType, std::string>& shader_paths)
// {
//     for (auto& shader_path : shader_paths)
//     {
//         if (compile(shader_path.second, ds_folder_path))
//         {
//             throw std::exception(
//                 (shader_path.second + " cant compile\n").c_str());
//         }
//     }
// }

// bool
// ShaderManager::compile(std::string_view file_path,
//                        const std::string& ds_folder_path)
// {
//     std::string command;

//     std::string compile_program_name = KERNEL.getFolderPath("shaders");

//     compile_program_name += "spirv/glslc.exe";
//     std::string file_name = util::Path::getName(file_path);

// #ifdef _WIN32
//     command = std::format("{} {} -o {}{}.spv", compile_program_name,
//     file_path,
//                           ds_folder_path, file_name);

//     // std::cout << command << '\n';
// #else

// #endif // _WIN32

//     int err_code = 0;

//     err_code = system(command.c_str());

//     return err_code;
// }

}; // namespace ars_graphics
