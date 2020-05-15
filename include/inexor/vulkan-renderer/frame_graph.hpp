#pragma once

#include <vulkan/vulkan_core.h>

#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace inexor::vulkan_renderer {

class RenderResource {
public:
    RenderResource() {}
};

class BufferResource : public RenderResource {
public:
    BufferResource() {

    }
};

class TextureResource : public RenderResource {
public:
    TextureResource() {

    }
};

/// @brief A single render stage in the frame graph.
/// @note Not to be confused with a vulkan render pass!
class RenderStage {
    friend class FrameGraph;

private:
    const std::string name;
    std::vector<const RenderResource *> writes;
    std::vector<const RenderResource *> reads;

    VkPipeline pipeline;
    std::vector<class Shader> shaders;

public:
    explicit RenderStage(std::string name) : name(std::move(name)) {}

    void writes_to(const RenderResource &resource);
    void reads_from(const RenderResource &resource);
};

class GraphicsStage : public RenderStage {
private:
    VkRenderPass render_pass;

public:
    explicit GraphicsStage(const std::string &name) : RenderStage(name) {}
};

template <typename Base, typename T, typename... Args>
using IsDerivedFromAndConstructible =
    std::enable_if_t<std::bool_constant<!std::is_same_v<Base, T> && std::is_base_of_v<Base, T> && std::is_constructible_v<T, Args...>>::value, int>;

class FrameGraph {
private:
    std::vector<RenderStage> stages;
    std::vector<RenderResource> resources;

    std::vector<RenderStage *> stage_stack;

public:
    template <typename T, typename... Args, IsDerivedFromAndConstructible<RenderStage, T, Args...> = 0>
    T &add(Args &&... args);

    template <typename T, typename... Args, IsDerivedFromAndConstructible<RenderResource, T, Args...> = 0>
    T &add(Args &&... args);

    void compile(const RenderResource &target);
};

template <typename T, typename... Args, IsDerivedFromAndConstructible<RenderStage, T, Args...>>
T &FrameGraph::add(Args &&... args) {
    return static_cast<T &>(stages.emplace_back(std::forward<Args>(args)...));
}

template <typename T, typename... Args, IsDerivedFromAndConstructible<RenderResource, T, Args...>>
T &FrameGraph::add(Args &&... args) {
    return static_cast<T &>(resources.emplace_back(std::forward<Args>(args)...));
}

} // namespace inexor::vulkan_renderer
