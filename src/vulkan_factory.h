#pragma once

#include <vector>
#include <functional>

#include "VkBootstrap.h"

namespace levin
{
    class VulkanFactory
    {
    public:
        typedef std::function<void(const vkb::Device & device)> destruction_callback_t;

    private:
        std::vector<destruction_callback_t> m_destruction_queue;
        const vkb::Device &m_device;

    public:
        VulkanFactory(const vkb::Device &device);
        VulkanFactory(const VulkanFactory &) = delete;

        ~VulkanFactory();

        void register_destruction(destruction_callback_t destroy) { m_destruction_queue.push_back(destroy); }

    protected:
        const vkb::Device &device() const { return m_device; }
    };
}