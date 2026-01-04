#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "model.hpp"

namespace ars_graphics
{

class ModelManager
{
public:
    void emplace(const std::string& name, Model&& model)
    {
        m_models.emplace(name, std::move(model));
    };

private:
    std::unordered_map<std::string, Model> m_models;
};

} // namespace ars_graphics
