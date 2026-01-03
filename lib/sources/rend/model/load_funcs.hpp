#pragma once
#include <iostream>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "../mesh/mesh_types.hpp"

#include "model.hpp"
#include "model_manager.hpp"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#include "tiny_gltf.h"

#define CHECK(val)      \
    if (false == (val)) \
    {                   \
        return false;   \
    }

#define CHECK_PTR(val)    \
    if (nullptr == (val)) \
    {                     \
        return false;     \
    }

namespace ars_graphics
{

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

/////////////////////////////////////////////////////////////////////////////////////////////
template <typename AttributeType>
bool
loadAttributes(const tinygltf::Model& model,
               const tinygltf::Primitive& primitive,
               std::vector<AttributeType>& attributes)
{
    std::cout << "Specialize loadAttributes\n";
    return false;
}

template <>
inline bool
loadAttributes(const tinygltf::Model& model,
               const tinygltf::Primitive& primitive,
               std::vector<attributes::Position3D>& attributes)
{
    return loadPositions(model, primitive, attributes);
}
template <>
inline bool
loadAttributes(const tinygltf::Model& model,
               const tinygltf::Primitive& primitive,
               std::vector<attributes::Normal>& attributes)
{
    return loadNormals(model, primitive, attributes);
}
template <>
inline bool
loadAttributes(const tinygltf::Model& model,
               const tinygltf::Primitive& primitive,
               std::vector<attributes::TextureCoord>& attributes)
{
    return loadTexCoords(model, primitive, attributes);
}

/////////////////////////////////////////////////////////////////////////////////////////////

bool
isBinary(const std::string& path);

bool
loadIndices(const tinygltf::Model& model,
            const tinygltf::Primitive& primitive,
            std::vector<uint32_t>& indices);

template <typename... VertexAttributes>
bool
loadAllAttributes(const tinygltf::Model& model,
                  const tinygltf::Primitive& primitive,
                  std::tuple<std::vector<VertexAttributes>...>& attributes)
{
    return (loadAttributes<VertexAttributes>(
                model, primitive,
                std::get<std::vector<VertexAttributes>>(attributes)) &&
            ...);
}

template <typename VertexType, typename AttrType>
struct CalculateVertexAttributeSutterTrick
{
    static void calculateVertexAttribute(VertexType& vertex,
                                         const AttrType& attribute_data)
    {
        std::cout << "Specialize CalculateVertexAttributeSutterTrick\n";
    }
};
template <typename VertexType>
struct CalculateVertexAttributeSutterTrick<VertexType, attributes::Position3D>
{
    static void calculateVertexAttribute(
        VertexType& vertex,
        const attributes::Position3D& attribute_data)
    {
        vertex.setPosition3D(
            *(reinterpret_cast<const glm::vec3*>(attribute_data.data())));
    }
};
template <typename VertexType>
struct CalculateVertexAttributeSutterTrick<VertexType, attributes::Normal>
{
    static void calculateVertexAttribute(
        VertexType& vertex,
        const attributes::Normal& attribute_data)
    {
        vertex.setNormal(
            *(reinterpret_cast<const glm::vec3*>(attribute_data.data())));
    }
};
template <typename VertexType>
struct CalculateVertexAttributeSutterTrick<VertexType, attributes::TextureCoord>
{
    static void calculateVertexAttribute(
        VertexType& vertex,
        const attributes::TextureCoord& attribute_data)
    {
        vertex.setCoord(
            *(reinterpret_cast<const glm::vec3*>(attribute_data.data())));
    }
};

template <typename VertexType, typename... VertexAttributes>
void
calculateAllVertexAttribute(
    VertexType& vertex,
    std::tuple<std::vector<VertexAttributes>...>& attributes,
    size_t index)
{
    (CalculateVertexAttributeSutterTrick<VertexType, VertexAttributes>::
         calculateVertexAttribute(
             vertex,
             std::get<std::vector<VertexAttributes>>(attributes)[index]),
     ...);
}

const tinygltf::Primitive*
getTrianglePrimitive(const std::vector<tinygltf::Primitive>& primitives);

template <typename MeshType, typename ModelManagerType>
bool
loadStandartModel(tinygltf::Model& model,
                  ModelManagerType& manager,
                  const std::string& path)
{
    Model<MeshType> result_model;

    for (auto&& gltf_mesh : model.meshes)
    {
        const tinygltf::Primitive* triangle_primitive =
            getTrianglePrimitive(gltf_mesh.primitives);

        CHECK_PTR(triangle_primitive)

        //////////////////////////////////////////

        typename MeshType::VertexType::TupleOfVectorAttributes
            vertex_attributes_vector_tuple;

        bool check_val = loadAllAttributes(model, *triangle_primitive,
                                           vertex_attributes_vector_tuple);

        CHECK(check_val)

        std::vector<uint32_t> indices;
        CHECK(loadIndices(model, *triangle_primitive, indices))

        size_t vertex_count =
            std::get<0>(vertex_attributes_vector_tuple).size();
        CHECK(vertex_count)
        //////////////////////////////////////////
        std::vector<typename MeshType::VertexType> vertices;
        vertices.reserve(vertex_count);

        for (size_t i = 0; i < vertex_count; ++i)
        {
            typename MeshType::VertexType vertex;

            calculateAllVertexAttribute(vertex, vertex_attributes_vector_tuple,
                                        i);

            vertices.push_back(std::move(vertex));
        }
        result_model.addMesh({std::move(indices), std::move(vertices)});
    }

    manager.emplace(path, std::move(result_model));
    return true;
}
} // namespace ars_graphics
