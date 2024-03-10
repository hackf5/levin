#pragma once

#include <memory>
#include <vector>

#include "device_components.h"
#include "descriptor_pool_components.h"
#include "swapchain_components.h"
#include "render_pass_components.h"
#include "graphics_pipeline_factory.h"

namespace levin
{
    class GraphicsPipelineComponents
    {
    private:
        GraphicsPipelineFactory m_factory;

        const VkPipelineLayout m_pipeline_layout;
        const VkPipeline m_pipeline;

        VkPipelineLayout create_pipeline_layout(
            const DescriptorPoolComponents &descriptor_components);

        VkPipeline create_pipeline(
            const SwapchainComponents &swapchain,
            const RenderPassComponents &render_pass);

        std::vector<VkPipelineShaderStageCreateInfo> create_shader_stages(
            VkShaderModule vert_module,
            VkShaderModule frag_module);

        VkPipelineVertexInputStateCreateInfo create_vertex_input_state(
            VkVertexInputBindingDescription &binding_description,
            VkVertexInputAttributeDescription *attribute_descriptions,
            uint32_t attribute_count);

        VkPipelineInputAssemblyStateCreateInfo create_input_assembly_state();

        VkPipelineViewportStateCreateInfo create_viewport_state(const SwapchainComponents &swapchain);

        VkPipelineRasterizationStateCreateInfo create_rasterization_state();

        VkPipelineMultisampleStateCreateInfo create_multisample_state();

        VkPipelineColorBlendStateCreateInfo create_color_blend_state();

        VkPipelineDynamicStateCreateInfo create_dynamic_state();

    public:
        GraphicsPipelineComponents(
            const DeviceComponents &device_components,
            const DescriptorPoolComponents &descriptor_components,
            const SwapchainComponents &swapchain,
            const RenderPassComponents &render_pass);
        GraphicsPipelineComponents(const GraphicsPipelineComponents &) = delete;

        operator VkPipeline() const { return m_pipeline; }

        VkPipelineLayout layout() const { return m_pipeline_layout; }
    };
}