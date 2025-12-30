#pragma once
#include <utility>

namespace ars_graphics
{

template <typename CT, typename T, typename Ret>
void
fillBoxesByPipelineImpl(CT* ct, std::pair<T*, Ret (CT::*)()>&& pair)
{
    *pair.first = (ct->*pair.second)();
}

template <typename CT, typename T, typename Ret>
void
fillBoxesByPipelineImpl(CT* ct, std::pair<T*, Ret (CT::*)() const>&& pair)
{
    *pair.first = (ct->*pair.second)();
}

template <typename CT, typename... TPack, typename... UPack>
void
fillBoxesByPipeline(CT* ct, std::pair<TPack*, UPack>&&... box_and_func_pack)
{
    (fillBoxesByPipelineImpl(
         ct, std::forward<std::pair<TPack*, UPack>>(box_and_func_pack)),
     ...);
}
} // namespace ars_graphics
