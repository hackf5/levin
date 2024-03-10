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
        const vkb::Device &m_device;

    protected:
        VulkanFactory(const vkb::Device &device);
        VulkanFactory(const VulkanFactory &) = delete;

        ~VulkanFactory();

        void register_destruction(destruction_callback_t destroy);

    public:
        const vkb::Device &get_device() const;
    };
}