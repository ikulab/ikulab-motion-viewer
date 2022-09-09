#pragma once

#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>

#include "../engine/renderEngine/renderEngine.hpp"

namespace ikura {
// Forward declearation ----------
class NativeWindow;

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    uint32_t id;

    static vk::VertexInputBindingDescription getBindingDescription() {
        vk::VertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = vk::VertexInputRate::eVertex;

        return bindingDescription;
    }

    static std::array<vk::VertexInputAttributeDescription, 3>
    getAttributeDescriptions() {
        std::array<vk::VertexInputAttributeDescription, 3>
            attributeDescriptions;
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = vk::Format::eR32Uint;
        attributeDescriptions[2].offset = offsetof(Vertex, id);

        return attributeDescriptions;
    }

    bool operator==(const Vertex &other) const {
        return (pos == other.pos && color == other.color && id == other.id);
    }
};

class RenderContent {
    std::shared_ptr<RenderEngine> renderEngine;
    std::weak_ptr<NativeWindow> nativeWindow;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    vk::Buffer vertexBuffer;
    vk::Buffer indexBuffer;
    vk::Buffer uniformBuffer;

    vk::DescriptorPool descriptorPool;
    vk::DescriptorSet descriptorSet;

    void createDefaultDescriptorSets();

  public:
    RenderContent(std::shared_ptr<NativeWindow> nativeWindow,
                  std::shared_ptr<RenderEngine> renderEngine);
    ~RenderContent();

    void setDefaultResources();
};
} // namespace ikura