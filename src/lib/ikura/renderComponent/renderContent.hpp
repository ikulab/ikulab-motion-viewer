#pragma once

#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>

#include "../common/renderPrimitiveTypes.hpp"
#include "../common/uniformBufferInfo.hpp"
#include "../engine/renderEngine/renderEngine.hpp"

namespace ikura {
// Forward declearation ----------
class NativeWindow;

class BufferResource {
  public:
    vk::Buffer buffer;
    VmaAllocation alloc;

    void release(VmaAllocator allocator);
};

class RenderContent {
    std::shared_ptr<RenderEngine> renderEngine;
    std::weak_ptr<NativeWindow> nativeWindow;

    std::vector<shapes::Vertex> vertices;
    std::vector<shapes::Index> indices;
    BufferResource vertexBufferResource;
    BufferResource indexBufferResource;
    // uniformBuffers[frame][set]
    std::vector<std::vector<BufferResource>> uniformBufferResources;

    vk::DescriptorPool descriptorPool;
    // descriptorSets[frame][set]
    std::vector<std::vector<vk::DescriptorSet>> descriptorSets;

    void createDefaultUniformBuffers();
    void createDefaultDescriptorSets();
    void assignDefaultVertexAndIndexArray();

  public:
    RenderContent(std::shared_ptr<NativeWindow> nativeWindow,
                  std::shared_ptr<RenderEngine> renderEngine);
    ~RenderContent();

    void setDefaultResources();

    void setVertices(const std::vector<shapes::Vertex> &vertices);
    void setIndices(const std::vector<shapes::Index> &indices);

    // Upload to GPU ----------
    void uploadVertexBuffer();
    void uploadIndexBuffer();
    void uploadVertexAndIndexBuffer();

    // Update non-cached data
    void updateUniformBuffer(shapes::ModelMatUBO &modelMatUBO,
                             shapes::SceneMatUBO &sceneMatUbo);

    // Getter ----------
    const vk::Buffer &getVertexBuffer() const;
    const vk::Buffer &getIndexBuffer() const;
    const std::vector<vk::DescriptorSet> &getDescriptorSets(int index);

    const std::vector<shapes::Index> &getIndices() const;
    const std::vector<shapes::Vertex> &getVertices() const;
};
} // namespace ikura