#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vulkan_engine.h"

#include "uniform_buffer_object.h"
#include "vertex.h"

#include "spdlog/spdlog.h"

using namespace levin;

const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
};

VulkanEngine::VulkanEngine(
    std::unique_ptr<VulkanContext> context)
    : m_context(std::move(context))
{
}

void VulkanEngine::run()
{
    spdlog::info("Vulkan Engine is running");

    load_vertexes();
    load_indexes();

    while (!m_context->window().should_close())
    {
        m_context->window().poll_events();
        draw_frame();
    }

    m_context->device().wait_idle();
}

void VulkanEngine::load_vertexes()
{
    auto staging_buffer = BufferCPUtoGPU(
        m_context->device(),
        sizeof(vertices[0]) * vertices.size(),
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    staging_buffer.copy_from((void *)vertices.data(), sizeof(vertices[0]) * vertices.size());
    m_context->vertex_buffer().copy_from(staging_buffer);
}

void VulkanEngine::load_indexes()
{
    auto staging_buffer = BufferCPUtoGPU(
        m_context->device(),
        sizeof(indices[0]) * indices.size(),
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    staging_buffer.copy_from((void *)indices.data(), sizeof(indices[0]) * indices.size());
    m_context->index_buffer().copy_from(staging_buffer);
}

void VulkanEngine::draw_frame()
{
    auto result = m_context->graphics_commands().acquire_next_image(
        m_context->current_frame(),
        m_context->swapchain());

    if (result == GraphicsResult::RecreateSwapchain)
    {
        recreate_swapchain();
        return;
    }

    update_uniform_buffer();

    execute_command_buffer();

    result = m_context->graphics_commands().present(
        m_context->current_frame(),
        m_context->swapchain());

    if (result == GraphicsResult::RecreateSwapchain)
    {
        recreate_swapchain();
    }

    m_context->next_frame();
}

void VulkanEngine::recreate_swapchain()
{
    m_context->window().wait_resize();
    m_context->device().wait_idle();

    VulkanContextBuilder builder(std::move(m_context));
    auto context = builder.configure_swapchain().build();
    m_context = std::move(context);
}

void VulkanEngine::execute_command_buffer()
{
    auto command_buffer = m_context->graphics_commands().begin(m_context->current_frame());

    auto extent = m_context->swapchain().extent();

    VkRenderPassBeginInfo render_pass_info {};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.renderPass = m_context->render_pass();
    render_pass_info.framebuffer = m_context->framebuffers().framebuffer(
        m_context->graphics_commands().image_index());
    render_pass_info.renderArea.offset = { 0, 0 };
    render_pass_info.renderArea.extent = extent;

    VkClearValue clear_color = { 0.0f, 0.0f, 0.0f, 1.0f };
    render_pass_info.clearValueCount = 1;
    render_pass_info.pClearValues = &clear_color;

    vkCmdBeginRenderPass(
        command_buffer,
        &render_pass_info,
        VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(
        command_buffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        m_context->graphics_pipeline());

    VkViewport viewport {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)extent.width;
    viewport.height = (float)extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(command_buffer, 0, 1, &viewport);

    VkRect2D scissor {};
    scissor.offset = { 0, 0 };
    scissor.extent = extent;
    vkCmdSetScissor(command_buffer, 0, 1, &scissor);

    VkBuffer vertex_buffers[] = { m_context->vertex_buffer() };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);

    vkCmdBindIndexBuffer(command_buffer, m_context->index_buffer(), 0, Vertex::vk_index_type);

    auto descriptor_set = m_context
        ->uniform_buffer_descriptor_set()
        .descriptor_set(m_context->current_frame());
    vkCmdBindDescriptorSets(
        command_buffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        m_context->graphics_pipeline().layout(),
        0,
        1,
        &descriptor_set,
        0,
        nullptr);

    vkCmdDrawIndexed(command_buffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(command_buffer);

    m_context->graphics_commands().end_and_submit(m_context->current_frame());
}

void VulkanEngine::update_uniform_buffer()
{
    static auto start_time = std::chrono::high_resolution_clock::now();

    auto current_time = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();

    UniformBufferObject ubo {};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(
        glm::vec3(2.0f, 2.0f, 2.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(
        glm::radians(45.0f),
        m_context->swapchain().extent().width / (float)m_context->swapchain().extent().height,
        0.1f,
        10.0f);
    ubo.proj[1][1] *= -1;

    m_context->uniform_buffer(m_context->current_frame()).copy_from(&ubo, sizeof(ubo));
}