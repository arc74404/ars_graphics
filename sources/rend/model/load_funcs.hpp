// #pragma once
// #include <iostream>
// #include <optional>
// #include <string>
// #include <tuple>
// #include <vector>

// #include "../material/material.hpp"
// #include "../vertex/vertex_types.hpp"

// #include "model.hpp"

// #define TINYGLTF_NO_STB_IMAGE_WRITE
// #include "tiny_gltf.h"

// #define CHECK(val)      \
//     if (false == (val)) \
//     {                   \
//         return false;   \
//     }

// #define CHECK_PTR(val)    \
//     if (nullptr == (val)) \
//     {                     \
//         return false;     \
//     }

// namespace ars_graphics
// {

// bool
// loadGLTFModel(const std::string& filepath,
//               bool is_binary,
//               tinygltf::Model& model);

// bool
// loadPositions(const tinygltf::Model& model,
//               const tinygltf::Primitive& primitive,
//               std::vector<attributes::Position3D>& positions);

// bool
// loadNormals(const tinygltf::Model& model,
//             const tinygltf::Primitive& primitive,
//             std::vector<attributes::Normal>& normals);

// bool
// loadTexCoords(const tinygltf::Model& model,
//               const tinygltf::Primitive& primitive,
//               std::vector<attributes::TextureCoord>& texcoords);
// bool
// loadColorRGBA(const tinygltf::Model& model,
//               const tinygltf::Primitive& primitive,
//               std::vector<attributes::ColorRGBA>& texcoords);

// /////////////////////////////////////////////////////////////////////////////////////////////
// template <typename AttributeType>
// bool
// loadAttributes(const tinygltf::Model& model,
//                const tinygltf::Primitive& primitive,
//                std::vector<AttributeType>& attributes)
// {
//     std::cout << "Specialize loadAttributes\n";
//     return false;
// }

// template <>
// inline bool
// loadAttributes(const tinygltf::Model& model,
//                const tinygltf::Primitive& primitive,
//                std::vector<attributes::Position3D>& attributes)
// {
//     return loadPositions(model, primitive, attributes);
// }
// template <>
// inline bool
// loadAttributes(const tinygltf::Model& model,
//                const tinygltf::Primitive& primitive,
//                std::vector<attributes::Normal>& attributes)
// {
//     return loadNormals(model, primitive, attributes);
// }
// template <>
// inline bool
// loadAttributes(const tinygltf::Model& model,
//                const tinygltf::Primitive& primitive,
//                std::vector<attributes::TextureCoord>& attributes)
// {
//     return loadTexCoords(model, primitive, attributes);
// }
// template <>
// inline bool
// loadAttributes(const tinygltf::Model& model,
//                const tinygltf::Primitive& primitive,
//                std::vector<attributes::ColorRGBA>& attributes)
// {
//     return loadColorRGBA(model, primitive, attributes);
// }

// /////////////////////////////////////////////////////////////////////////////////////////////

// bool
// isBinary(const std::string& path);

// bool
// loadIndices(const tinygltf::Model& model,
//             const tinygltf::Primitive& primitive,
//             std::vector<uint32_t>& indices);

// template <typename... VertexAttributes>
// bool
// loadAllAttributes(
//     const tinygltf::Model& model,
//     const tinygltf::Primitive& primitive,
//     std::tuple<std::vector<VertexAttributes>...>& attributes_storage)
// {
//     return (loadAttributes<VertexAttributes>(
//                 model, primitive,
//                 std::get<std::vector<VertexAttributes>>(attributes_storage))
//                 &&
//             ...);
// }

// template <typename VertexType, typename AttrType>
// struct CalculateVertexAttributeSutterTrick
// {
//     static void calculateVertexAttribute(VertexType& vertex,
//                                          const AttrType& attribute_data)
//     {
//         std::cout << "Specialize CalculateVertexAttributeSutterTrick\n";
//     }
// };
// template <typename VertexType>
// struct CalculateVertexAttributeSutterTrick<VertexType,
// attributes::Position3D>
// {
//     static void calculateVertexAttribute(
//         VertexType& vertex,
//         const attributes::Position3D& attribute_data)
//     {
//         glm::vec3 pos =
//             *(reinterpret_cast<const glm::vec3*>(attribute_data.data()));
//         vertex.setPosition3D(pos);
//     }
// };
// template <typename VertexType>
// struct CalculateVertexAttributeSutterTrick<VertexType, attributes::Normal>
// {
//     static void calculateVertexAttribute(
//         VertexType& vertex,
//         const attributes::Normal& attribute_data)
//     {
//         vertex.setNormal(
//             *(reinterpret_cast<const glm::vec3*>(attribute_data.data())));
//     }
// };
// template <typename VertexType>
// struct CalculateVertexAttributeSutterTrick<VertexType,
// attributes::TextureCoord>
// {
//     static void calculateVertexAttribute(
//         VertexType& vertex,
//         const attributes::TextureCoord& attribute_data)
//     {
//         glm::vec2 pos =
//             *(reinterpret_cast<const glm::vec2*>(attribute_data.data()));
//         vertex.setCoord(pos);
//     }
// };

// template <typename VertexType, typename... VertexAttributes>
// void
// calculateAllVertexAttribute(
//     VertexType& vertex,
//     std::tuple<std::vector<VertexAttributes>...>& attributes,
//     size_t index)
// {
//     (CalculateVertexAttributeSutterTrick<VertexType, VertexAttributes>::
//          calculateVertexAttribute(
//              vertex,
//              std::get<std::vector<VertexAttributes>>(attributes)[index]),
//      ...);
// }

// bool
// hasIndices(const tinygltf::Primitive& primitive);

// vk::PrimitiveTopology
// convertGltfTopologyToVulkan(int gltf_topology);

// template <typename VertexType>
// bool
// loadPrimitive(const tinygltf::Model& gltf_model,
//               const tinygltf::Primitive& gltf_primitive,
//               Mesh& mesh,
//               const std::vector<NonUpdatebleMaterial>& materials)
// {
//     std::vector<uint32_t> indices;
//     if (hasIndices(gltf_primitive))
//     {
//         CHECK(loadIndices(gltf_model, gltf_primitive, indices))
//     }
//     typename VertexType::TupleOfVectorAttributes attrs;

//     CHECK(loadAllAttributes(gltf_model, gltf_primitive, attrs))

//     // -- vertices -- //

//     size_t vertex_count = std::get<0>(attrs).size();
//     CHECK(vertex_count)
//     std::vector<VertexType> vertices;
//     vertices.reserve(vertex_count);

//     for (size_t i = 0; i < vertex_count; ++i)
//     {
//         VertexType vertex;

//         calculateAllVertexAttribute(vertex, attrs, i);

//         vertices.push_back(std::move(vertex));
//     }

//     // -------------- //

//     mesh.addPrimitive(
//         Primitive{std::move(indices), vertices,
//                   convertGltfTopologyToVulkan(gltf_primitive.mode),
//                   &(materials[gltf_primitive.material])});
//     return true;
// }

// template <typename... VertexTypes>
// bool
// loadMesh(tinygltf::Model& gltf_model,
//          tinygltf::Mesh& gltf_mesh,
//          Model& my_model,
//          const std::vector<NonUpdatebleMaterial>& materials)
// {
//     Mesh mesh;
//     for (auto&& primitive : gltf_mesh.primitives)
//     {
//         bool check_val = (loadPrimitive<VertexTypes>(gltf_model, primitive,
//                                                      mesh, materials) ||
//                           ...);

//         CHECK(check_val)
//     }
//     my_model.addMesh(std::move(mesh));

//     return true;
// }
// } // namespace ars_graphics
