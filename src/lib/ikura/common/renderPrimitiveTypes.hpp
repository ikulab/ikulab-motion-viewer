#pragma once

#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>

namespace ikura {

class Vertex {
  public:
    virtual size_t getDataSize() { return 0; };

    static void *convetToDataVector(std::vector<Vertex> vertices);
};

class BasicVertex : public Vertex {
  public:
    struct Data {
        glm::vec3 pos;
        glm::vec3 color;
        uint32_t id;
    } data;

    BasicVertex(glm::vec3 pos, glm::vec3 color, uint32_t id) {
        data.pos = pos;
        data.color = color;
        data.id = id;
    }
    BasicVertex(const BasicVertex &basicVertex) {
        Data newData;
        newData.pos = basicVertex.data.pos;
        newData.color = basicVertex.data.color;
        newData.id = basicVertex.data.id;

        data = newData;
    }

    size_t getDataSize() override { return sizeof(Data); }

    static vk::VertexInputBindingDescription getBindingDescription() {
        vk::VertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(BasicVertex);
        bindingDescription.inputRate = vk::VertexInputRate::eVertex;

        return bindingDescription;
    }

    std::vector<
        vk::VertexInputAttributeDescription> static getAttributeDescriptions() {
        std::vector<vk::VertexInputAttributeDescription> attributeDescriptions(
            3);
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions[0].offset = offsetof(Data, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions[1].offset = offsetof(Data, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = vk::Format::eR32Uint;
        attributeDescriptions[2].offset = offsetof(Data, id);

        return attributeDescriptions;
    }

    bool operator==(const BasicVertex &other) const {
        return (data.pos == other.data.pos && data.color == other.data.color &&
                data.id == other.data.id);
    }
};

typedef uint32_t Index;
typedef Index BasicIndex;
// todo: delete this
typedef uint32_t JointID;
} // namespace ikura