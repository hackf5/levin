#pragma once

#include <vector>
#include <functional>

#include "VkBootstrap.h"

namespace levin
{
    class VulkanFactory
    {
    public:
        typedef std::function<void()> destruction_callback_t;

    private:
        std::vector<std::function<void()>> m_destruction_queue;
        vkb::Device *const m_device;

    protected:
        VulkanFactory(vkb::Device &device);
        ~VulkanFactory();

        void register_destruction(destruction_callback_t destroy);

    public:
        vkb::Device &get_device();
    };
}