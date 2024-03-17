#pragma once

#include <memory>
#include <vector>

#include "device.h"
#include "descriptor_set_layout.h"
#include "swapchain.h"
#include "render_pass.h"
#include "shader_module.h"

namespace levin
{
    class GraphicsPipeline
    {
    private:
        const Device &m_device;

        const ShaderModule &m_shader_modules;

        const VkPipelineLayout m_pipeline_layout;
        const VkPipeline m_pipeline;

        VkPipelineLayout create_pipeline_layout(
            const DescriptorSetLayout &descriptor_set_layout);

        VkPipeline create_pipeline(
            const Swapchain &swapchain,
            const RenderPass &render_pass);

        std::vector<VkPipelineShaderStageCreateInfo> create_shader_stages();

        VkPipelineInputAssemblyStateCreateInfo create_input_assembly_state();

        VkPipelineViewportStateCreateInfo create_viewport_state(const Swapchain &swapchain);

        VkPipelineRasterizationStateCreateInfo create_rasterization_state();

        VkPipelineMultisampleStateCreateInfo create_multisample_state();

        VkPipelineColorBlendAttachmentState create_color_blend_attachment_state();

        VkPipelineColorBlendStateCreateInfo create_color_blend_state(
            const VkPipelineColorBlendAttachmentState &color_blend_attachment_state);

        std::vector<VkDynamicState> create_dynamic_states();

        VkPipelineDynamicStateCreateInfo create_dynamic_state(
            const std::vector<VkDynamicState> &dynamic_states);

    public:
        GraphicsPipeline(
            const Device &device,
            const DescriptorSetLayout &descriptor_set_layout,
            const ShaderModule &shader_modules,
            const Swapchain &swapchain,
            const RenderPass &render_pass);
        GraphicsPipeline(const GraphicsPipeline &) = delete;
        ~GraphicsPipeline();

        operator VkPipeline() const { return m_pipeline; }

        VkPipelineLayout layout() const { return m_pipeline_layout; }
    };
}