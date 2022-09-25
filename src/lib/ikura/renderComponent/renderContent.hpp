#pragma once

#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>

#include "../common/renderPrimitiveTypes.hpp"
#include "../common/uniformBufferInfo.hpp"
#include "../engine/renderEngine/renderEngine.hpp"

namespace ikura {
class BufferResource {
  public:
    vk::Buffer buffer;
    VmaAllocation alloc;

    void release(VmaAllocator allocator);
};

class RenderContent {
  protected:
    RenderContent(std::shared_ptr<RenderEngine> renderEngine,
                  vk::DescriptorSetLayout descriptorSetLayout, int numOfFrames);

    // Variables ==========
    std::shared_ptr<RenderEngine> renderEngine;

    // Index / Vertex ----------
    std::vector<Vertex> vertices;
    std::vector<Index> indices;

    // Buffers ----------
    BufferResource vertexBufferResource;
    BufferResource indexBufferResource;
    // uniformBuffers[frame][set]
    std::vector<std::vector<BufferResource>> uniformBufferResources;

    // about DescriptorSet ----------
    vk::DescriptorPool descriptorPool;
    // descriptorSets[frame][set]
    std::vector<std::vector<vk::DescriptorSet>> descriptorSets;
    vk::DescriptorSetLayout descriptorSetLayout;

    // Properties ----------
    int numOfFrames;

  public:
    ~RenderContent();

    // Upload to GPU ----------
    void uploadVertexBuffer();
    void uploadIndexBuffer();
    void uploadVertexAndIndexBuffer();

    // Getter ----------
    const vk::Buffer &getVertexBuffer() const;
    const vk::Buffer &getIndexBuffer() const;
    const std::vector<vk::DescriptorSet> &getDescriptorSets(int index);

    const std::vector<Index> &getIndices() const;
    const std::vector<Vertex> &getVertices() const;
};
} // namespace ikura