#pragma once

#include <iostream>
#include <vector>

#include "../material/material.hpp"

namespace ars_graphics
{
class MaterialStorage
{
public:
    template <template <typename, typename...> typename Cont, typename... Args>
    std::vector<Material>& pushMaterials(Cont<Material, Args...>&& cont)
    {
        auto& dest = m_storage.emplace_back();

        for (size_t i = 0; i < cont.size(); ++i)
        {
            dest.emplace_back(std::move(cont[i]));
        }

        return dest;
    }

private:
    std::vector<std::vector<Material>> m_storage;
};

}; // namespace ars_graphics
