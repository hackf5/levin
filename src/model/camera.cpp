#include "camera.h"

#include "spdlog/spdlog.h"

using namespace levin;

Camera::Camera(
    const Device &device,
    const DescriptorPool &descriptor_pool,
    const DescriptorSetLayout &descriptor_set_layout):
    m_uniform_buffer(
        device,
        descriptor_pool,
        descriptor_set_layout,
        sizeof(UniformBlock),
        UniformBuffer::CAMERA),
    m_uniform_block({ glm::identity<glm::mat4>(), glm::identity<glm::mat4>() }),
    m_position(glm::zero<glm::vec3>()),
    m_target(glm::zero<glm::vec3>()),
    m_fov(45.0f),
    m_aspect_ratio(1.0f),
    m_near(0.1f),
    m_far(10.0f)
{
}