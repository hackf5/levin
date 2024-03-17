#pragma once

#include <memory>
#include <vector>

#include "device.h"
#include "descriptor_set_layout.h"
#include "shader_module_components.h"
#include "swapchain_components.h"
#include "render_pass_components.h"
#include "graphics_pipeline_factory.h"

namespace levin
{
    class GraphicsPipelineComponents
    {
    private:
        GraphicsPipelineFactory m_factory;
        const ShaderModuleComponents &m_shader_modules;

        const VkPipelineLayout m_pipeline_layout;
        const VkPipeline m_pipeline;

        VkPipelineLayout create_pipeline_layout(
            const DescriptorSetLayout &descriptor_set_layout);

        VkPipeline create_pipeline(
            const SwapchainComponents &swapchain,
            const RenderPassComponents &render_pass);

        std::vector<VkPipelineShaderStageCreateInfo> create_shader_stages();

        VkPipelineInputAssemblyStateCreateInfo create_input_assembly_state();

        VkPipelineViewportStateCreateInfo create_viewport_state(const SwapchainComponents &swapchain);

        VkPipelineRasterizationStateCreateInfo create_rasterization_state();

        VkPipelineMultisampleStateCreateInfo create_multisample_state();

        VkPipelineColorBlendAttachmentState create_color_blend_attachment_state();

        VkPipelineColorBlendStateCreateInfo create_color_blend_state(
            const VkPipelineColorBlendAttachmentState &color_blend_attachment_state);

        std::vector<VkDynamicState> create_dynamic_states();

        VkPipelineDynamicStateCreateInfo create_dynamic_state(
            const std::vector<VkDynamicState> &dynamic_states);

    public:
        GraphicsPipelineComponents(
            const Device &device,
            const DescriptorSetLayout &descriptor_set_layout,
            const ShaderModuleComponents &shader_modules,
            const SwapchainComponents &swapchain,
            const RenderPassComponents &render_pass);
        GraphicsPipelineComponents(const GraphicsPipelineComponents &) = delete;

        operator VkPipeline() const { return m_pipeline; }

        VkPipelineLayout layout() const { return m_pipeline_layout; }
    };
}