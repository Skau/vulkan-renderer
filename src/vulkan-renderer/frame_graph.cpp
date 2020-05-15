#include "inexor/vulkan-renderer/frame_graph.hpp"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <cassert>
#include <functional>

namespace inexor::vulkan_renderer {

void RenderStage::writes_to(const RenderResource &resource) {
    writes.push_back(&resource);
}

void RenderStage::reads_from(const RenderResource &resource) {
    reads.push_back(&resource);
}

void FrameGraph::compile(const RenderResource &target) {
    // TODO(*): Create a generic graph lib for use in other inexor modules (e.g visual scripting)
    // TODO(*): Many opportunities for optimisation

    // TODO(yeetari): Maybe store like this in the first place?
    std::unordered_map<const RenderResource *, std::vector<RenderStage *>> writers;
    for (auto &stage : stages) {
        for (const auto *resource : stage.writes) {
            writers[resource].push_back(&stage);
        }
    }

    // Post order depth first search
    // NOTE: Doesn't do any colouring! Only works on acyclic graphs!
    // TODO(yeetari): Move away from recursive dfs algo
    std::function<void(RenderStage *)> dfs = [&](RenderStage *stage) {
        dfs(stage);
        stage_stack.push_back(stage);
    };

    // TODO(yeetari): Will there be more than one writer to the back buffer? Maybe with blending?
    for (auto *writer : writers[&target]) {
        stage_stack.push_back(writer);
    }

    // DFS starting from back buffer
    for (auto *stage : stage_stack) {
        dfs(stage);
    }

    // Reverse post order
    std::reverse(stage_stack.begin(), stage_stack.end());

#ifndef NDEBUG
    spdlog::debug("Frame graph stage order:");
    for (auto *stage : stage_stack) {
        spdlog::debug(stage->name);
    }
#endif

    // Physical resource allocation
    // TODO(yeetari): Resource aliasing algo
    for (const auto &resource : resources) {
        // TODO(yeetari): Use vma to allocate buffers
    }

    // Render pass creation
    // NOTE: Each render stage(after merging and reordering) maps to a vulkan render pass
    for (const auto &stage : stages) {
        VkRenderPassCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    }
}

} // namespace inexor::vulkan_renderer
