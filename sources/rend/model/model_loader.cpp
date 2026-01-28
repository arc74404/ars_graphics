#include "model_loader.hpp"

#include <set>

#include "../textures/texture_data.hpp"

#include "stb_image.h"

namespace ars_graphics
{
void
ModelLoader::load(PipelineManager& pipeline_manager,
                  const vk::RenderPass& render_pass,
                  TextureStorage& texture_storage,
                  const TextureCreater& texture_creater,
                  MaterialStorage& material_storage,
                  const MaterialCreater& material_creater,
                  const std::vector<std::string>& paths,
                  std::unordered_map<std::string, Model>& model_storage) const
{
    TextureDataStorage texture_data_storage;

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

        if (false == loadImpl(pipeline_manager, render_pass, gltf_model,
                              model_storage, texture_data_storage,
                              texture_storage, texture_creater,
                              material_storage, material_creater, path))
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
ModelLoader::loadImpl(PipelineManager& pipeline_manager,
                      const vk::RenderPass& render_pass,
                      tinygltf::Model& gltf_model,
                      std::unordered_map<std::string, Model>& model_storage,
                      TextureDataStorage& texture_data_storage,
                      TextureStorage& texture_storage,
                      const TextureCreater& texture_creater,
                      MaterialStorage& material_storage,
                      const MaterialCreater& material_creater,
                      const std::string& path) const
{
    Model res_model;

    std::vector<TextureData> textures_data;

    if (false == loadTextures(gltf_model.textures, gltf_model, textures_data))
    {
        std::cout << "Failed load textures: " << path << '\n';
        return false;
    }

    std::vector<TextureData>& realoc_textures_data =
        texture_data_storage.pushDataTextures(std::move(textures_data));

    std::vector<ars_graphics::MaterialData> materials_data;

    if (false == loadMaterials(gltf_model.materials, realoc_textures_data,
                               texture_creater, texture_storage,
                               materials_data))
    {
        std::cout << "Failed load materials: " << path << '\n';
        return false;
    }

    std::vector<ars_graphics::Material> convert_data =
        material_creater.convertToMaterials(materials_data);

    std::vector<Material>& realoc_materials =
        material_storage.pushMaterials(std::move(convert_data));

    int index = 0;
    for (auto&& gltf_mesh : gltf_model.meshes)
    {
        if (!(index == 2 || index == 3))
        {
            index++;
            continue;
        }
        index++;
        bool checker = loadMesh<VerticesPriorityListPack>(
            pipeline_manager, render_pass, gltf_model, gltf_mesh, res_model,
            realoc_materials);

        CHECK(checker)
    }

    std::cout << index << '\n';
    model_storage.emplace(path, std::move(res_model));

    return true;
}

std::optional<TextureData>
ModelLoader::loadTexture(const tinygltf::Image& image,
                         const tinygltf::Model& gltf_model) const
{
    if (image.bufferView >= 0)
    {
        TextureData texture_data{};

        auto&& bufferView = gltf_model.bufferViews[image.bufferView];
        auto&& buffer     = gltf_model.buffers[bufferView.buffer];

        const uint8_t* data = buffer.data.data() + bufferView.byteOffset;
        size_t data_size    = bufferView.byteLength;

        int width, height, channels;

        unsigned char* pixels = stbi_load_from_memory(
            data, static_cast<int>(data_size), &width, &height, &channels, 0);

        if (nullptr == pixels)
        {
            return std::nullopt;
        }

        texture_data.setData(pixels, width * height * channels, width, height,
                             channels, stbi_image_free);
        return texture_data;
    }
    return std::nullopt;
}

bool
ModelLoader::loadTextures(const std::vector<tinygltf::Texture>& gltf_textures,
                          const tinygltf::Model& gltf_model,
                          std::vector<TextureData>& textures_data) const
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

        auto&& texture_data = loadTexture(image, gltf_model);

        if (false == texture_data.has_value())
        {
            std::cout << "Failed to load texture " << cur_index
                      << " (image: " << texture.source << ")\n";
            return false;
        }

        textures_data.emplace_back(std::move(texture_data.value()));

        ++cur_index;
    }

    return true;
}

static bool
hasTexture(int val)
{
    return val >= 0;
}

static void
setupAlphaMode(MaterialData& material_data,
               const std::string& alpha_mode,
               float alpha_cutoff)
{
    if (alpha_mode == "MASK")
    {
        material_data.m_alpha_mode   = AlphaMode::MASK;
        material_data.m_alpha_cutoff = alpha_cutoff;
    }
    else if (alpha_mode == "BLEND")
    {
        material_data.m_alpha_mode = AlphaMode::BLEND;
    }
    else
    {
        material_data.m_alpha_mode = AlphaMode::TOTAL_OPAQUE;
    }
}

bool
ModelLoader::loadMaterials(
    const std::vector<tinygltf::Material>& gltf_materials,
    const std::vector<TextureData>& textures_data,
    const TextureCreater& texture_creater,
    TextureStorage& texture_storage,
    std::vector<MaterialData>& materials_data) const
{
    materials_data.reserve(gltf_materials.size());

    for (auto&& gltf_material : gltf_materials)
    {
        MaterialData material_data;
        PBRParameters pbr_params{};

        pbr_params.setAlbedo(
            gltf_material.pbrMetallicRoughness.baseColorFactor);
        pbr_params.setEmissiveFactor(gltf_material.emissiveFactor);
        pbr_params.m_ao_strength = gltf_material.occlusionTexture.strength;

        pbr_params.m_metallic =
            gltf_material.pbrMetallicRoughness.metallicFactor;
        pbr_params.m_roughness =
            gltf_material.pbrMetallicRoughness.roughnessFactor;

        std::list<Texture> textures;
        std::vector<uint16_t> texture_inds;

        auto createTexture =
            [&textures, &texture_creater, &textures_data, &texture_inds,
             &texture_storage](size_t index,
                               const Texture*& texture_for_dummy_init,
                               Texture::TextureType texture_type)
        {
            if (hasTexture(index))
            {
                texture_inds.emplace_back(index);
                auto&& res = texture_creater.createPBRTexture(
                    textures_data[index], texture_type);
                if (false == res.has_value())
                {
                    texture_for_dummy_init =
                        texture_storage.getDummy(texture_type);
                    return;
                }
                textures.push_back(std::move(res.value()));
            }
            else
            {
                texture_for_dummy_init = texture_storage.getDummy(texture_type);
            }
        };
        createTexture(gltf_material.pbrMetallicRoughness.baseColorTexture.index,
                      pbr_params.m_albedo_map, Texture::TextureType::ALBEDO);

        createTexture(
            gltf_material.pbrMetallicRoughness.metallicRoughnessTexture.index,
            pbr_params.m_metallic_roughness_map,
            Texture::TextureType::METALLIC_ROUGHNESS);

        createTexture(gltf_material.normalTexture.index,
                      pbr_params.m_normal_map, Texture::TextureType::NORMAL);

        createTexture(gltf_material.emissiveTexture.index,
                      pbr_params.m_emissive_map,
                      Texture::TextureType::EMISSIVE);

        createTexture(gltf_material.occlusionTexture.index, pbr_params.m_ao_map,
                      Texture::TextureType::OCCLUSION);

        auto& realloc_textures =
            texture_storage.pushTextures(std::move(textures));

        for (size_t i = 0; i < texture_inds.size(); ++i)
        {
            if (gltf_material.pbrMetallicRoughness.baseColorTexture.index ==
                texture_inds[i])
            {
                pbr_params.m_albedo_map = &realloc_textures[i];
            }
            else if (gltf_material.pbrMetallicRoughness.metallicRoughnessTexture
                         .index == texture_inds[i])
            {
                pbr_params.m_metallic_roughness_map = &realloc_textures[i];
            }
            else if (gltf_material.normalTexture.index == texture_inds[i])
            {
                pbr_params.m_normal_map = &realloc_textures[i];
            }
            else if (gltf_material.emissiveTexture.index == texture_inds[i])
            {
                pbr_params.m_emissive_map = &realloc_textures[i];
            }
            else if (gltf_material.occlusionTexture.index == texture_inds[i])
            {
                pbr_params.m_ao_map = &realloc_textures[i];
            }
        }

        material_data.m_pbrparams = std::move(pbr_params);

        material_data.m_double_sided = gltf_material.doubleSided;

        setupAlphaMode(material_data, gltf_material.alphaMode,
                       gltf_material.alphaCutoff);

        materials_data.push_back(std::move(material_data));
    }

    return true;
}

} // namespace ars_graphics
