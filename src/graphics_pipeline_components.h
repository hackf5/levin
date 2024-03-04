#pragma once

#include <memory>

#include "device_components.h"
#include "swapchain_components.h"
#include "graphics_pipeline_factory.h"

namespace levin
{
    class GraphicsPipelineComponents
    {
    private:
        GraphicsPipelineFactory m_factory;

        VkPipelineLayout m_pipeline_layout;
        VkPipeline m_pipeline;

        void init_pipeline(
            VkShaderModule vert_module,
            VkShaderModule frag_module,
            VkRenderPass render_pass);

    public:
        GraphicsPipelineComponents(
            const std::shared_ptr<DeviceComponents> &device_components,
            VkRenderPass render_pass);

        VkPipeline get_pipeline() const
        {
            return m_pipeline;
        }
    };
}