#include "model_loader.hpp"

#include "texture_data.hpp"

namespace ars_graphics
{
void
ModelLoader::load(const std::vector<std::string>& paths,
                  ModelManager& model_manager) const
{
    for (auto&& path : paths)
    {
        if (false == fileOpenSuccess(path))
        {
            std::cout << "Failed open file: " << path << '\n';
            continue;
        }
        tinygltf::Model gltf_model;
        if (false == loadGLTFModel(path, isBinary(path), gltf_model))
        {
            std::cout << "Failed loadGLTFModel: " << path << '\n';
            continue;
        }

        if (false == loadImpl(gltf_model, model_manager, path))
        {
            std::cout << "Failed load model: " << path << '\n';
        }
    }
}

bool
ModelLoader::fileOpenSuccess(const std::string& path) const
{
    std::ifstream file(path, std::ios::binary);
    if (false == file.is_open())
    {
        return false;
    }
    file.close();
    return true;
}

bool
ModelLoader::loadImpl(tinygltf::Model& gltf_model,
                      ModelManager& model_manager,
                      const std::string& path) const
{
    Model res_model;

    if (false == loadTextures(gltf_model.textures, gltf_model))
    {
        std::cout << "Failed load textures: " << path << '\n';
        return false;
    }

    for (auto&& gltf_mesh : gltf_model.meshes)
    {
        bool checker = loadMesh<PrimitivesPriorityListPack>(
            gltf_model, gltf_mesh, res_model, nullptr);

        CHECK(checker)
    }
    model_manager.emplace(path, std::move(res_model));
    return true;
}

bool
ModelLoader::loadTexture(const tinygltf::Image& image,
                         const tinygltf::Model& gltf_model) const
{
    if (image.bufferView >= 0)
    {
        TextureData texture_data{};

        const auto& bufferView = gltf_model.bufferViews[image.bufferView];
        const auto& buffer     = gltf_model.buffers[bufferView.buffer];

        const uint8_t* data = buffer.data.data() + bufferView.byteOffset;
        size_t data_size    = bufferView.byteLength;

        int width, height, channels;
        unsigned char* pixels = stbi_load_from_memory(
            data, static_cast<int>(data_size), &width, &height, &channels, 0);

        CHECK_PTR(pixels)
        texture_data.setData(pixels, width * height * channels, width, height,
                             channels);
        return true;
    }
    return false;
}

bool
ModelLoader::loadTextures(const std::vector<tinygltf::Texture>& gltf_textures,
                          const tinygltf::Model& gltf_model) const
{
    if (gltf_textures.empty() || gltf_model.images.empty())
    {
        return true;
    }

    size_t cur_index = 0;
    for (auto&& texture : gltf_textures)
    {
        if (texture.source < 0 || texture.source >= gltf_model.images.size())
        {
            continue;
        }

        auto&& image = gltf_model.images[texture.source];

        if (false == loadTexture(image, gltf_model))
        {
            std::cout << "Failed to load texture " << cur_index
                      << " (image: " << texture.source << ")\n";
            return false;
        }
        ++cur_index;
    }

    return true;
}
} // namespace ars_graphics
