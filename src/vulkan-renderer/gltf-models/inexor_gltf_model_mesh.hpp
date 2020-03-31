#pragma once

#include "inexor_bounding_box.hpp"
#include "inexor_gltf_primitive.hpp"

#include "../uniform-buffer/vk_uniform_buffer.hpp"
#include "../gltf-models/inexor_gltf_model_uniform_buffer.hpp"

#include <vector>
#include <memory>


namespace inexor {
namespace vulkan_renderer {
namespace glTF2_models {


	struct InexorModelMesh
	{
		std::vector<std::shared_ptr<InexorModelPrimitive>> primitives;

		BoundingBox bb;

		BoundingBox aabb;

		std::shared_ptr<InexorUniformBuffer> uniform_buffer;

		InexorModelStandardUniformBufferBlock uniform_block;


		InexorModelMesh()
		{
		}


		~InexorModelMesh()
		{
		}

		
		/// @brief Sets the model matrix.
		/// @param mat [in] The input matrix.
		void set_matrix(const glm::mat4& mat)
		{
			uniform_block.matrix = mat;
		}


		/// @brief Sets the bounding box of the model.
		/// @param min [in] The minimum vector (edge of the bounding box)
		/// @param max [in] The maximum vector (edge of the bounding box)
		void set_bounding_box(glm::vec3 min, glm::vec3 max)
		{
			bb.min = min;
			bb.max = max;
			bb.valid = true;
		}
	};

};
};
};