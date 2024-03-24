#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vulkan_context_builder.h"
#include "vulkan_engine.h"

#include "spdlog/spdlog.h"

#include "vulkan/image.h"

using namespace levin;

const std::vector<levin::Vertex> vertexes = {
    {
        .pos = {-0.5f, -0.5f, 0.0f},
        .uv = {1.0f, 0.0f},
        .color = {1.0f, 0.0f, 0.0f, 1.0f},
    },
    {
        {0.5f, -0.5f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
    },
    {
        {0.5f, 0.5f, 0.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
    },
    {
        {-0.5f, 0.5f, 0.0f},
        {1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f},
    },
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

    load_scene();

    while (!m_context->window().should_close())
    {
        m_context->window().poll_events();
        draw_frame();
    }

    m_context->device().wait_idle();
}

void VulkanEngine::load_scene()
{
    m_context->graphics_buffers().load_vertexes(vertexes);
    m_context->graphics_buffers().load_indexes(indexes);

    m_image = std::make_unique<Image>(
        m_context->device(),
        m_context->sampler(),
        m_context->adhoc_queues(),
        "george.png");

    auto &camera = m_context->scene().camera();
    camera.position() = glm::vec3(2.0f, 2.0f, 2.0f);
    camera.target() = glm::vec3(0.0f, 0.0f, 0.0f);
    camera.clip_far() = 10.0f;
    camera.clip_near() = 0.1f;
    camera.fov() = 45.0f;
    camera.aspect_ratio() = m_context->swapchain().aspect_ratio();

    std::vector<Primitive> primitives = {
        {0, static_cast<uint32_t>(indexes.size())}
    };

    auto &root_node = m_context->scene().model().root_node();
    auto mesh1 = std::make_unique<Mesh>(
        m_context->device(),
        primitives,
        m_image.get());
    auto &child1 = root_node.add_child(std::move(mesh1));
    child1.translation() = glm::vec3(-0.5f, 0.0f, 0.0f);

    auto mesh2 = std::make_unique<Mesh>(
        m_context->device(),
        primitives,
        m_image.get());
    auto &child2 = root_node.add_child(std::move(mesh2));
    child2.translation() = glm::vec3(0.5f, 0.0f, 0.0f);
}

void VulkanEngine::recreate_swapchain()
{
    m_context->window().wait_resize();
    m_context->device().wait_idle();

    auto context = VulkanContextBuilder(std::move(m_context))
        .add_swapchain()
        .add_render_pass()
        .add_framebuffers()
        .add_graphics_pipeline()
        .add_gui()
        .build();
    m_context = std::move(context);

    auto &camera = m_context->scene().camera();
    camera.aspect_ratio() = m_context->swapchain().aspect_ratio();
}

void VulkanEngine::draw_frame()
{
    m_context->gui().begin_frame();

    auto framebuffer = m_context->graphics_queue().prepare_framebuffer(
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

    if (!m_context->graphics_queue().present_framebuffer())
    {
        recreate_swapchain();
    }

    next_frame();
}

void VulkanEngine::render(VkFramebuffer framebuffer)
{
    auto command_buffer = m_context->graphics_queue().begin_command();

    m_context->render_pass().begin(command_buffer, framebuffer);

    m_context->graphics_pipeline().bind(command_buffer);
    m_context->swapchain().clip(command_buffer);
    m_context->graphics_buffers().bind(command_buffer);
    m_context->scene().render(command_buffer, m_context->graphics_pipeline());
    m_context->gui().render(command_buffer);

    m_context->render_pass().end(command_buffer);

    m_context->graphics_queue().submit_command();
}

void VulkanEngine::update_uniform_buffer()
{
    static auto start_time = std::chrono::high_resolution_clock::now();

    auto current_time = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();

    auto rotation = glm::quat_cast(
        glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

    auto &node = m_context->scene().model().root_node();

    node.rotation() = rotation;
    for (auto &child : node.children())
    {
        child->rotation() = rotation;
    }

    m_context->scene().flush();
}