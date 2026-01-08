#pragma once

#include <vector>

#include "../material/material.hpp"

namespace ars_graphics
{
class MaterialStorage
{
public:
    template <typename Cont>
    std::vector<Material>& pushMaterials(Cont&& cont)
    {
        auto& dest = m_storage.emplace_back();
        dest.reserve(cont.size());
        for (auto&& el : cont)
        {
            dest.emplace_back(std::move(el));
        }
        return dest;
    }

private:
    std::vector<std::vector<Material>> m_storage;
};

}; // namespace ars_graphics
