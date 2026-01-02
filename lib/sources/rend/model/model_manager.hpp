#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "model.hpp"

namespace ars_graphics
{

template <typename MeshT>
using ModelMap = std::unordered_map<std::string, Model<MeshT>>;

template <typename... MeshTypes>
class ModelManager
{
public:
    template <typename MeshT>
    void emplace(const std::string& name, Model<MeshT>&& model)
    {
        if (isNameReserved(name))
        {
            throw std::logic_error("Model name '" + name +
                                   "' is already reserved");
        }

        m_reserved_names.emplace(name);

        std::get<ModelMap<MeshT>>(m_meshes).emplace(name, std::move(model));
    };

    template <typename MeshT>
    const Model<MeshT>* const get(const std::string& name)
    {
        const auto& mesh_map = std::get<ModelMap<MeshT>>(m_meshes);

        auto it = mesh_map.find(name);

        if (it == mesh_map.end())
        {
            return nullptr;
        }

        return it->second.get();
    };

private:
    bool isNameReserved(const std::string& name)
    {
        return m_reserved_names.end() != std::find(m_reserved_names.begin(),
                                                   m_reserved_names.end(),
                                                   name);
    }

    std::unordered_set<std::string> m_reserved_names;

    std::tuple<ModelMap<MeshTypes>...> m_meshes;
};

} // namespace ars_graphics
