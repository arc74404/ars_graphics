#pragma once
#include <utility>
#include <vector>

#include "glm/mat4x4.hpp"

namespace ars_graphics
{

template <bool need_alloc, typename T, typename U>
void
pushDataToTheEnd(std::vector<T>& dest, const std::vector<U>& src)
{
    size_t dest_old_size = dest.size() * sizeof(T);
    if constexpr (need_alloc)
    {
        dest.resize(dest.size() + src.size() * sizeof(U) / sizeof(T));
    }
    memcpy(reinterpret_cast<char*>(dest.data()) + dest_old_size, src.data(),
           src.size() * sizeof(U));
}

glm::mat4
getFinalMatrix(const std::vector<double>& rotation,
               const std::vector<double>& translation,
               const std::vector<double>& scale,
               const std::vector<double>& matrix);

glm::mat4
convertVectorToMatrix(const std::vector<double>& data);

} // namespace ars_graphics
