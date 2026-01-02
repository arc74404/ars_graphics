#include "load_funcs.hpp"

#include <iostream>

bool
ars_graphics::loadGLTFModel(const std::string& filepath,
                            bool is_binary,
                            tinygltf::Model& model)
{
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool success = false;

    if (is_binary)
    {
        success = loader.LoadBinaryFromFile(&model, &err, &warn, filepath);
    }
    else
    {
        success = loader.LoadASCIIFromFile(&model, &err, &warn, filepath);
    }

    if (!warn.empty())
    {
        std::cout << "GLTF warning (" << filepath << "): " << warn << std::endl;
    }

    if (!err.empty())
    {
        std::cerr << "GLTF error (" << filepath << "): " << err << std::endl;
    }

    if (!success)
    {
        std::cerr << "Failed to load GLTF: " << filepath << std::endl;
        return false;
    }

    return true;
}

bool
ars_graphics::loadPositions(const tinygltf::Model& model,
                            const tinygltf::Primitive& primitive,
                            std::vector<attributes::Position3D>& positions)
{
    return loadAttributes(model, primitive, "POSITION", positions);
}

bool
ars_graphics::loadNormals(const tinygltf::Model& model,
                          const tinygltf::Primitive& primitive,
                          std::vector<attributes::Normal>& normals)
{
    return loadAttributes(model, primitive, "NORMAL", normals);
}

bool
ars_graphics::loadTexCoords(const tinygltf::Model& model,
                            const tinygltf::Primitive& primitive,
                            std::vector<attributes::TextureCoord>& texcoords)
{
    const char* texcoord_names[] = {"TEXCOORD_0", "TEXCOORD", "TEXCOORD0"};

    for (const char* name : texcoord_names)
    {
        if (loadAttributes<attributes::TextureCoord>(model, primitive, name,
                                                     texcoords))
        {
            auto it = primitive.attributes.find(name);
            if (it != primitive.attributes.end())
            {
                const auto& accessor = model.accessors[it->second];
                if (accessor.type == TINYGLTF_TYPE_VEC2 &&
                    accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT)
                {
                    return true;
                }
            }
            texcoords.clear();
        }
    }

    return false;
}

static size_t
getComponentSize(int componentType)
{
    switch (componentType)
    {
        case TINYGLTF_COMPONENT_TYPE_BYTE:
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
            return 1;

        case TINYGLTF_COMPONENT_TYPE_SHORT:
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
            return 2;

        case TINYGLTF_COMPONENT_TYPE_INT:
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
        case TINYGLTF_COMPONENT_TYPE_FLOAT:
            return 4;

        case TINYGLTF_COMPONENT_TYPE_DOUBLE:
            return 8;

        default:
            return 0;
    }
}

bool
ars_graphics::loadIndices(const tinygltf::Model& model,
                          const tinygltf::Primitive& primitive,
                          std::vector<uint32_t>& indices)
{
    if (primitive.indices < 0)
    {
        auto it = primitive.attributes.find("POSITION");
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

        indices.resize(accessor.count);
        for (size_t i = 0; i < accessor.count; ++i)
        {
            indices[i] = static_cast<uint32_t>(i);
        }

        return true;
    }
    int indices_idx = primitive.indices;
    if (indices_idx < 0 ||
        indices_idx >= static_cast<int>(model.accessors.size()))
    {
        return false;
    }

    const auto& accessor = model.accessors[indices_idx];

    if (accessor.type != TINYGLTF_TYPE_SCALAR)
    {
        return false;
    }

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
    size_t data_end =
        data_start + accessor.count * getComponentSize(accessor.componentType);
    if (data_end > buffer.data.size())
    {
        return false;
    }

    const uint8_t* data = buffer.data.data() + data_start;
    indices.resize(accessor.count);

    switch (accessor.componentType)
    {
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
            std::memcpy(indices.data(), data,
                        accessor.count * sizeof(uint32_t));
            break;

        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
            for (size_t i = 0; i < accessor.count; ++i)
            {
                indices[i] = reinterpret_cast<const uint16_t*>(data)[i];
            }
            break;

        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
            for (size_t i = 0; i < accessor.count; ++i)
            {
                indices[i] = static_cast<uint32_t>(data[i]);
            }
            break;

        default:
            indices.clear();
            return false;
    }

    return true;
}

namespace
{
const tinygltf::Primitive*
getTrianglePrimitive(const std::vector<tinygltf::Primitive>& primitives)
{
    const tinygltf::Primitive* triangle_primitive = nullptr;

    for (auto&& primitive : primitives)
    {
        if (primitive.mode == TINYGLTF_MODE_TRIANGLES)
        {
            triangle_primitive = &primitive;
            break;
        }
    }
    return triangle_primitive;
}
} // namespace

#define CHECK(val)           \
    if (false == (val))      \
    {                        \
        return std::nullopt; \
    }

#define CHECK_PTR(val)       \
    if (nullptr == (val))    \
    {                        \
        return std::nullopt; \
    }

std::optional<ars_graphics::Model<ars_graphics::mesh_types::Standard3D>>
ars_graphics::loadStandardModel(const std::string& filepath, bool is_binary)
{
    tinygltf::Model model;

    CHECK(loadGLTFModel(filepath, is_binary, model))

    CHECK(model.meshes.size())

    Model<mesh_types::Standard3D> result_model;

    for (auto&& gltf_mesh : model.meshes)
    {
        const tinygltf::Primitive* triangle_primitive =
            getTrianglePrimitive(gltf_mesh.primitives);

        CHECK_PTR(triangle_primitive)

        //////////////////////////////////////////

        std::vector<attributes::Position3D> positions;
        std::vector<attributes::Normal> normals;
        std::vector<attributes::TextureCoord> texcoords;
        std::vector<uint32_t> indices;

        CHECK(loadPositions(model, *triangle_primitive, positions))
        CHECK(loadNormals(model, *triangle_primitive, normals))
        CHECK(loadTexCoords(model, *triangle_primitive, texcoords))
        CHECK(loadIndices(model, *triangle_primitive, indices))

        size_t vertex_count = positions.size();
        CHECK(vertex_count)

        //////////////////////////////////////////
        std::vector<mesh_types::Standard3D::VertexType> vertices;
        vertices.reserve(vertex_count);

        for (size_t i = 0; i < vertex_count; ++i)
        {
            mesh_types::Standard3D::VertexType vertex;

            if (i < positions.size())
            {
                vertex.setPosition3D(
                    *(reinterpret_cast<const glm::vec3*>(positions[i].data())));
            }

            if (i < normals.size())
            {
                vertex.setNormal(
                    *(reinterpret_cast<const glm::vec3*>(normals[i].data())));
            }

            if (i < texcoords.size())
            {
                vertex.setNormal(
                    *(reinterpret_cast<const glm::vec3*>(texcoords[i].data())));
            }

            vertices.push_back(std::move(vertex));

            result_model.addMesh({std::move(indices), std::move(vertices)});
        }
    }

    return result_model;
}
