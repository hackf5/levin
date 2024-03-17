#include "graphics_pipeline.h"

#include "swapchain_factory.h"
#include "vertex.h"

#include "spdlog/spdlog.h"

using namespace levin;

GraphicsPipeline::GraphicsPipeline(
    const Device &device,
    const DescriptorSetLayout &descriptor_set_layout,
    const ShaderModule &shader_modules,
    const Swapchain &swapchain,
    const RenderPass &render_pass):
    m_factory(device),
    m_shader_modules(shader_modules),
    m_pipeline_layout(create_pipeline_layout(descriptor_set_layout)),
    m_pipeline(create_pipeline(swapchain, render_pass))
{
}

VkPipelineLayout GraphicsPipeline::create_pipeline_layout(
    const DescriptorSetLayout &descriptor_set_layout)
{
    VkDescriptorSetLayout descriptor_set_layouts[] = { descriptor_set_layout };
    VkPipelineLayoutCreateInfo pipeline_layout_info = {};
    pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_info.setLayoutCount = 1;
    pipeline_layout_info.pSetLayouts = descriptor_set_layouts;
    pipeline_layout_info.pushConstantRangeCount = 0;
    pipeline_layout_info.pPushConstantRanges = nullptr;

    return m_factory.create_pipeline_layout(pipeline_layout_info);

}

VkPipeline GraphicsPipeline::create_pipeline(
    const Swapchain &swapchain,
    const RenderPass &render_pass)
{
    auto shader_stages = create_shader_stages();

    VertexInputState vertex_input_state(0, { VertexComponent::Position, VertexComponent::Color });

    auto input_assembly_state = create_input_assembly_state();
    auto viewport_state = create_viewport_state(swapchain);
    auto rasterization_state = create_rasterization_state();
    auto multisampling_state = create_multisample_state();
    auto color_blend_attachment = create_color_blend_attachment_state();
    auto color_blend_state = create_color_blend_state(color_blend_attachment);
    auto dynamic_states = create_dynamic_states();
    auto dynamic_state = create_dynamic_state(dynamic_states);

    VkGraphicsPipelineCreateInfo pipeline_info = {};
    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount = static_cast<uint32_t>(shader_stages.size());
    pipeline_info.pStages = shader_stages.data();
    pipeline_info.pVertexInputState = &vertex_input_state.pipeline;
    pipeline_info.pInputAssemblyState = &input_assembly_state;
    pipeline_info.pViewportState = &viewport_state;
    pipeline_info.pRasterizationState = &rasterization_state;
    pipeline_info.pMultisampleState = &multisampling_state;
    pipeline_info.pColorBlendState = &color_blend_state;
    pipeline_info.pDynamicState = &dynamic_state;
    pipeline_info.layout = m_pipeline_layout;
    pipeline_info.renderPass = render_pass;
    pipeline_info.subpass = 0;
    pipeline_info.basePipelineHandle = VK_NULL_HANDLE;

    auto pipeline = m_factory.create_pipeline(pipeline_info);
    return pipeline;
}

std::vector<VkPipelineShaderStageCreateInfo> GraphicsPipeline::create_shader_stages()
{
    VkPipelineShaderStageCreateInfo vert_stage_info = {};
    vert_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vert_stage_info.module = m_shader_modules.get("vert");
    vert_stage_info.pName = "main";

    VkPipelineShaderStageCreateInfo frag_stage_info = {};
    frag_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_stage_info.module = m_shader_modules.get("frag");
    frag_stage_info.pName = "main";

    return { vert_stage_info, frag_stage_info };
}

VkPipelineInputAssemblyStateCreateInfo GraphicsPipeline::create_input_assembly_state()
{
    VkPipelineInputAssemblyStateCreateInfo result = {};
    result.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    result.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    result.primitiveRestartEnable = VK_FALSE;

    return result;
}

VkPipelineViewportStateCreateInfo GraphicsPipeline::create_viewport_state(const Swapchain &swapchain)
{
    VkPipelineViewportStateCreateInfo result = {};
    result.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    result.viewportCount = 1;
    result.pViewports = &swapchain.viewport();
    result.scissorCount = 1;
    result.pScissors = &swapchain.scissor();

    return result;
}

VkPipelineRasterizationStateCreateInfo GraphicsPipeline::create_rasterization_state()
{
    VkPipelineRasterizationStateCreateInfo result = {};
    result.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    result.depthClampEnable = VK_FALSE;
    result.rasterizerDiscardEnable = VK_FALSE;
    result.polygonMode = VK_POLYGON_MODE_FILL;
    result.lineWidth = 1.0f;
    result.cullMode = VK_CULL_MODE_BACK_BIT;
    result.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    result.depthBiasEnable = VK_FALSE;

    return result;
}

VkPipelineMultisampleStateCreateInfo GraphicsPipeline::create_multisample_state()
{
    VkPipelineMultisampleStateCreateInfo result = {};
    result.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    result.sampleShadingEnable = VK_FALSE;
    result.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    return result;
}

VkPipelineColorBlendAttachmentState GraphicsPipeline::create_color_blend_attachment_state()
{
    VkPipelineColorBlendAttachmentState result = {};
    result.blendEnable = VK_FALSE;
    result.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
        | VK_COLOR_COMPONENT_G_BIT
        | VK_COLOR_COMPONENT_B_BIT
        | VK_COLOR_COMPONENT_A_BIT;

    return result;
}

VkPipelineColorBlendStateCreateInfo GraphicsPipeline::create_color_blend_state(
    const VkPipelineColorBlendAttachmentState &color_blend_attachment)
{
    VkPipelineColorBlendStateCreateInfo result = {};
    result.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    result.logicOpEnable = VK_FALSE;
    result.logicOp = VK_LOGIC_OP_COPY;
    result.attachmentCount = 1;
    result.pAttachments = &color_blend_attachment;
    result.blendConstants[0] = 0.0f;
    result.blendConstants[1] = 0.0f;
    result.blendConstants[2] = 0.0f;
    result.blendConstants[3] = 0.0f;

    return result;
}

std::vector<VkDynamicState> GraphicsPipeline::create_dynamic_states()
{
    return
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
}

VkPipelineDynamicStateCreateInfo GraphicsPipeline::create_dynamic_state(
    const std::vector<VkDynamicState> &dynamic_states)
{
    VkPipelineDynamicStateCreateInfo result = {};
    result.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    result.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
    result.pDynamicStates = dynamic_states.data();

    return result;
}