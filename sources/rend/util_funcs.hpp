#pragma once
#include <utility>

namespace ars_graphics
{

// template <typename CT, typename T, typename Ret>
// void
// fillBoxesByPipelineImpl(CT* ct, std::pair<T*, Ret (CT::*)()>&& pair)
// {
//     *pair.first = (ct->*pair.second)();
// }

// template <typename CT, typename T, typename Ret>
// void
// fillBoxesByPipelineImpl(CT* ct, std::pair<T*, Ret (CT::*)() const>&& pair)
// {
//     *pair.first = (ct->*pair.second)();
// }

// template <typename CT, typename... TPack, typename... UPack>
// void
// fillBoxesByPipeline(CT* ct, std::pair<TPack*, UPack>&&... box_and_func_pack)
// {
//     (fillBoxesByPipelineImpl(
//          ct, std::forward<std::pair<TPack*, UPack>>(box_and_func_pack)),
//      ...);
// }

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

} // namespace ars_graphics
