#include "util_funcs.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

glm::mat4
ars_graphics::getFinalMatrix(const std::vector<double>& rotation,
                             const std::vector<double>& translation,
                             const std::vector<double>& scale,
                             const std::vector<double>& matrix)
{
    if (matrix.size() == 16)
    {
        return convertVectorToMatrix(matrix);
    }

    glm::mat4 transform = glm::mat4(1.0f);

    if (scale.size() == 3)
    {
        glm::vec3 scaleVec(static_cast<float>(scale[0]),
                           static_cast<float>(scale[1]),
                           static_cast<float>(scale[2]));
        transform = glm::scale(transform, scaleVec);
    }

    if (rotation.size() == 4)
    {
        glm::quat quat(
            static_cast<float>(rotation[3]), static_cast<float>(rotation[0]),
            static_cast<float>(rotation[1]), static_cast<float>(rotation[2]));
        transform *= glm::mat4_cast(quat);
    }

    if (translation.size() == 3)
    {
        glm::vec3 translationVec(static_cast<float>(translation[0]),
                                 static_cast<float>(translation[1]),
                                 static_cast<float>(translation[2]));
        transform = glm::translate(glm::mat4(1.0f), translationVec) * transform;
    }

    return transform;
}

glm::mat4
ars_graphics::convertVectorToMatrix(const std::vector<double>& data)
{
    if (data.size() != 16)
    {
        return glm::mat4(1.0f);
    }

    glm::mat4 mat;
    for (uint8_t col = 0; col < 4; col++)
    {
        for (uint8_t row = 0; row < 4; row++)
        {
            mat[col][row] = static_cast<float>(data[col * 4 + row]);
        }
    }
    return mat;
}
