#pragma once

#include <vector>
#include <functional>

#include "VkBootstrap.h"

namespace levin
{
    class VulkanFactory
    {
    protected:
        typedef std::function<void(const vkb::Device &device)> destruction_callback_t;

    private:
        std::vector<destruction_callback_t> m_destruction_queue;
        const vkb::Device &m_device;

    public:
        VulkanFactory(const vkb::Device &device);
        VulkanFactory(const VulkanFactory &) = delete;

        ~VulkanFactory();

    protected:
        const vkb::Device &device() const { return m_device; }

        void register_destruction(destruction_callback_t destroy) { m_destruction_queue.push_back(destroy); }
    };
}