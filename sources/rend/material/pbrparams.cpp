#include "pbrparams.hpp"

namespace ars_graphics
{
int
PBRParameters::countTextures() const
{
    return (m_albedo_map != nullptr) + (m_normal_map != nullptr) +
           (m_ao_map != nullptr) + (m_emissive_map != nullptr) +
           (m_metallic_roughness_map != nullptr);
}

} // namespace ars_graphics
