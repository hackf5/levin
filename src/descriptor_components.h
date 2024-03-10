#pragma once

#include <memory>
#include <vector>

#include "device_components.h"
#include "descriptor_factory.h"

namespace levin
{
    class DescriptorComponents
    {
    private:
        DescriptorFactory m_factory;

        VkDescriptorSetLayout m_descriptor_set_layout;
        VkDescriptorPool m_descriptor_pool;

        void init_descriptor_set_layout();

        void init_descriptor_pool();

    public:
        DescriptorComponents(const DeviceComponents& device);
        DescriptorComponents(const DescriptorComponents&) = delete;

        VkDescriptorSetLayout layout() const { return m_descriptor_set_layout; }

        VkDescriptorPool pool() const { return m_descriptor_pool; }
    };
}