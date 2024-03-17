#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vulkan_context_builder.h"
#include "vulkan_engine.h"
#include "uniform_buffer_object.h"

#include "spdlog/spdlog.h"

using namespace levin;

const std::vector<levin::Vertex> vertexes = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<levin::Vertex::index_t> indexes = {
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

    load_model();

    while (!m_context->window().should_close())
    {
        m_context->window().poll_events();
        draw_frame();
    }

    m_context->device().wait_idle();
}

void VulkanEngine::load_model()
{
    m_context->model().load_vertexes(vertexes);
    m_context->model().load_indexes(indexes);

    std::vector<std::unique_ptr<Primitive>> primitives;
    primitives.push_back(std::make_unique<Primitive>(0, indexes.size()));
    m_context->model().load_primitives(primitives);

    std::vector<Primitive*> mesh_primitives;
    for (auto &primitive : m_context->model().primitives())
    {
        mesh_primitives.push_back(primitive);
    }

    auto mesh = std::make_unique<Mesh>(
        m_context->device(),
        m_context->descriptor_pool(),
        m_context->descriptor_set_layout(),
        mesh_primitives);

    m_context->model().root_node(
        std::make_unique<Node>(
            nullptr,
            std::move(mesh)));
}

void VulkanEngine::recreate_swapchain()
{
    m_context->window().wait_resize();
    m_context->device().wait_idle();

    VulkanContextBuilder builder(std::move(m_context));
    auto context = builder
        .configure_swapchain()
        .configure_render_pass()
        .configure_framebuffers()
        .configure_graphics_pipeline()
        .build();
    m_context = std::move(context);
}

void VulkanEngine::draw_frame()
{
    auto framebuffer = m_context
        ->graphics_commands()
        .prepare_framebuffer(
            m_current_frame,
            m_context->swapchain(),
            m_context->framebuffers());
    if (!framebuffer)
    {
        recreate_swapchain();
        return;
    }

    update_uniform_buffer();
    render(framebuffer);

    if (!m_context->graphics_commands().present_framebuffer())
    {
        recreate_swapchain();
    }

    next_frame();
}

void VulkanEngine::render(VkFramebuffer framebuffer)
{
    auto command_buffer = m_context
        ->graphics_commands()
        .begin_command();

    auto extent = m_context->swapchain().extent();

    VkRenderPassBeginInfo render_pass_info {};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.renderPass = m_context->render_pass();
    render_pass_info.framebuffer = framebuffer;
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

    m_context->model().bind(command_buffer);

    // m_context->uniform_buffer_descriptor_set(m_current_frame).bind(command_buffer, m_context->graphics_pipeline());

    m_context->model().draw(command_buffer, m_context->graphics_pipeline(), m_current_frame);

    vkCmdEndRenderPass(command_buffer);

    m_context
        ->graphics_commands()
        .submit_command();
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

    m_context->uniform_buffer(m_current_frame).copy_from(&ubo, sizeof(ubo));
    auto node = m_context->model().root_node();
    node->rotation() = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    node->update(m_current_frame);
}