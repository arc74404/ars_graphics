#pragma once
#include <optional>
#include <string>

#include "../mesh/mesh_types.hpp"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define STB_IMAGE_IMPLEMENTATION
#include "model.hpp"
#include "tiny_gltf.h"

namespace ars_graphics
{
std::optional<Model<mesh_types::Standard3D>>
loadStandardModel(const std::string& filepath, bool is_binary);

bool
loadGLTFModel(const std::string& filepath,
              bool is_binary,
              tinygltf::Model& model);

bool
loadPositions(const tinygltf::Model& model,
              const tinygltf::Primitive& primitive,
              std::vector<attributes::Position3D>& positions);

bool
loadNormals(const tinygltf::Model& model,
            const tinygltf::Primitive& primitive,
            std::vector<attributes::Normal>& normals);

bool
loadTexCoords(const tinygltf::Model& model,
              const tinygltf::Primitive& primitive,
              std::vector<attributes::TextureCoord>& texcoords);

bool
loadIndices(const tinygltf::Model& model,
            const tinygltf::Primitive& primitive,
            std::vector<uint32_t>& indices);

template <typename AttributeType>
bool
loadAttributes(const tinygltf::Model& model,
               const tinygltf::Primitive& primitive,
               const std::string& attributeName,
               std::vector<AttributeType>& attributes)
{
    auto it = primitive.attributes.find(attributeName);
    if (it == primitive.attributes.end())
    {
        return false;
    }

    int accessor_idx = it->second;
    if (accessor_idx < 0 ||
        accessor_idx >= static_cast<int>(model.accessors.size()))
    {
        return false;
    }

    const auto& accessor = model.accessors[accessor_idx];

    if (accessor.bufferView < 0 ||
        accessor.bufferView >= static_cast<int>(model.bufferViews.size()))
    {
        return false;
    }

    const auto& buffer_view = model.bufferViews[accessor.bufferView];

    if (buffer_view.buffer < 0 ||
        buffer_view.buffer >= static_cast<int>(model.buffers.size()))
    {
        return false;
    }

    const auto& buffer = model.buffers[buffer_view.buffer];

    size_t data_start = buffer_view.byteOffset + accessor.byteOffset;
    size_t data_end   = data_start + accessor.count * sizeof(AttributeType);
    if (data_end > buffer.data.size())
    {
        return false;
    }

    const AttributeType* data =
        reinterpret_cast<const AttributeType*>(buffer.data.data() + data_start);

    attributes.assign(data, data + accessor.count);

    return true;
}

} // namespace ars_graphics
