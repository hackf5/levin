#pragma once

#include <memory>
#include <vector>

#include "device_components.h"
#include "descriptor_components.h"
#include "swapchain_components.h"
#include "graphics_pipeline_factory.h"

namespace levin
{
    class GraphicsPipelineComponents
    {
    private:
        DescriptorComponents const * const m_descriptor_components;

        GraphicsPipelineFactory m_factory;

        VkPipelineLayout m_pipeline_layout;
        VkPipeline m_pipeline;

        void init_pipeline(
            VkShaderModule vert_module,
            VkShaderModule frag_module,
            VkRenderPass render_pass);

        void init_pipeline_layout();

        std::vector<VkPipelineShaderStageCreateInfo> create_shader_stages(
            VkShaderModule vert_module,
            VkShaderModule frag_module);

        VkPipelineVertexInputStateCreateInfo create_vertex_input_state(
            VkVertexInputBindingDescription &binding_description,
            VkVertexInputAttributeDescription *attribute_descriptions,
            uint32_t attribute_count);

        VkPipelineInputAssemblyStateCreateInfo create_input_assembly_state();

        VkViewport create_viewport();

        VkRect2D create_scissor();

        VkPipelineViewportStateCreateInfo create_viewport_state(VkViewport &viewport, VkRect2D &scissor);

        VkPipelineRasterizationStateCreateInfo create_rasterization_state();

        VkPipelineMultisampleStateCreateInfo create_multisample_state();

        VkPipelineColorBlendStateCreateInfo create_color_blend_state();

        VkPipelineDynamicStateCreateInfo create_dynamic_state();

    public:
        GraphicsPipelineComponents(
            const DeviceComponents &device_components,
            const DescriptorComponents &descriptor_components,
            VkRenderPass render_pass);
        GraphicsPipelineComponents(const GraphicsPipelineComponents &) = delete;

        VkPipeline get_pipeline() const { return m_pipeline; }

        VkPipelineLayout get_pipeline_layout() const { return m_pipeline_layout; }
    };
}