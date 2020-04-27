#pragma once

#include <glm/glm.hpp>

namespace inexor::vulkan_renderer::gltf_model {

// Changing this value here also requires changing it in the vertex shader.
constexpr std::uint32_t MAX_NUM_JOINTS = 128u;

/// @brief Inexor's standard uniform buffer block for glTF 2.0 models.
struct StandardUniformBufferBlock {
    glm::mat4 matrix;

    glm::mat4 joint_matrix[MAX_NUM_JOINTS]{};

    float joint_count{0};
};

} // namespace inexor::vulkan_renderer::gltf_model