#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include "window.h"
#include "device.h"
#include "swapchain.h"
#include "render_pass.h"

namespace levin
{
    class Gui
    {
    private:
        const Device &m_device;
        VkDescriptorPool m_descriptor_pool;

        VkDescriptorPool create_descriptor_pool();

        static void check_vk_result(VkResult result);

    public:
        Gui(
            const Window &window,
            const Device &device,
            const RenderPass &render_pass);
        ~Gui();

        void begin_frame() const;
        void render(VkCommandBuffer command_buffer) const;
    };
} //