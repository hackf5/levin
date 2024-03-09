#include "graphics_pipeline_components.h"

#include "swapchain_factory.h"
#include "vertex.h"

#include "spdlog/spdlog.h"

using namespace levin;

const std::vector<VkDynamicState> dynamic_states =
{
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
};

const VkPipelineColorBlendAttachmentState color_blend_attachment =
{
    .blendEnable = VK_FALSE,
    .colorWriteMask = VK_COLOR_COMPONENT_R_BIT
        | VK_COLOR_COMPONENT_G_BIT
        | VK_COLOR_COMPONENT_B_BIT
        | VK_COLOR_COMPONENT_A_BIT,
};

GraphicsPipelineComponents::GraphicsPipelineComponents(
    const std::shared_ptr<DeviceComponents> &device_components,
    const std::shared_ptr<DescriptorComponents> &descriptor_components,
    VkRenderPass render_pass):
    m_descriptor_components(descriptor_components),
    m_factory(GraphicsPipelineFactory(device_components.get()->get_device()))
{
    spdlog::info("Initializing Graphics Pipeline Components");

    try
    {
        init_pipeline_layout();

        auto vert_module = m_factory.create_shader_module("vert");
        auto frag_module = m_factory.create_shader_module("frag");

        init_pipeline(vert_module, frag_module, render_pass);

        m_factory.destroy_shader_module(frag_module);
        m_factory.destroy_shader_module(vert_module);
    }
    catch (const std::runtime_error &e)
    {
        spdlog::error("Failed to initialize graphics pipeline: {}", e.what());
        this->~GraphicsPipelineComponents();
        throw;
    }
}

void GraphicsPipelineComponents::init_pipeline_layout()
{
    auto layout = m_descriptor_components->layout();
    VkPipelineLayoutCreateInfo pipeline_layout_info = {};
    pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_info.setLayoutCount = 1;
    pipeline_layout_info.pSetLayouts = &layout;
    pipeline_layout_info.pushConstantRangeCount = 0;
    pipeline_layout_info.pPushConstantRanges = nullptr;

    m_pipeline_layout = m_factory.create_pipeline_layout(pipeline_layout_info);
}

void GraphicsPipelineComponents::init_pipeline(
    VkShaderModule vert_module,
    VkShaderModule frag_module,
    VkRenderPass render_pass)
{
    auto shader_stages = create_shader_stages(vert_module, frag_module);

    auto vertex_binding_description = Vertex::get_binding_description();
    auto vertex_attribute_descriptions = Vertex::get_attribute_descriptions();
    auto vertex_input_state = create_vertex_input_state(
        vertex_binding_description,
        vertex_attribute_descriptions.data(),
        vertex_attribute_descriptions.size());

    auto input_assembly_state = create_input_assembly_state();
    auto viewport = create_viewport();
    auto scissor = create_scissor();
    auto viewport_state = create_viewport_state(viewport, scissor);
    auto rasterization_state = create_rasterization_state();
    auto multisampling_state = create_multisample_state();
    auto color_blend_state = create_color_blend_state();
    auto dynamic_state = create_dynamic_state();

    VkGraphicsPipelineCreateInfo pipeline_info = {};
    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount = static_cast<uint32_t>(shader_stages.size());
    pipeline_info.pStages = shader_stages.data();
    pipeline_info.pVertexInputState = &vertex_input_state;
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

    m_pipeline = m_factory.create_pipeline(pipeline_info);
}

std::vector<VkPipelineShaderStageCreateInfo> GraphicsPipelineComponents::create_shader_stages(
    VkShaderModule vert_module,
    VkShaderModule frag_module)
{
    VkPipelineShaderStageCreateInfo vert_stage_info = {};
    vert_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vert_stage_info.module = vert_module;
    vert_stage_info.pName = "main";

    VkPipelineShaderStageCreateInfo frag_stage_info = {};
    frag_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_stage_info.module = frag_module;
    frag_stage_info.pName = "main";

    return { vert_stage_info, frag_stage_info };
}

VkPipelineVertexInputStateCreateInfo GraphicsPipelineComponents::create_vertex_input_state(
    VkVertexInputBindingDescription &binding_description,
    VkVertexInputAttributeDescription *attribute_descriptions,
    uint32_t attribute_count)
{
    VkPipelineVertexInputStateCreateInfo result = {};

    result.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    result.vertexBindingDescriptionCount = 1;
    result.pVertexBindingDescriptions = &binding_description;
    result.vertexAttributeDescriptionCount = attribute_count;
    result.pVertexAttributeDescriptions = attribute_descriptions;

    return result;
}

VkPipelineInputAssemblyStateCreateInfo GraphicsPipelineComponents::create_input_assembly_state()
{
    VkPipelineInputAssemblyStateCreateInfo result = {};
    result.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    result.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    result.primitiveRestartEnable = VK_FALSE;

    return result;
}

VkViewport GraphicsPipelineComponents::create_viewport()
{
    auto extent = SwapchainFactory::get_config().extent;

    VkViewport result = {};
    result.x = 0.0f;
    result.y = 0.0f;
    result.width = static_cast<float>(extent.width);
    result.height = static_cast<float>(extent.height);
    result.minDepth = 0.0f;
    result.maxDepth = 1.0f;

    return result;
}

VkRect2D GraphicsPipelineComponents::create_scissor()
{
    auto extent = SwapchainFactory::get_config().extent;

    VkRect2D result = {};
    result.offset = { 0, 0 };
    result.extent = extent;

    return result;
}

VkPipelineViewportStateCreateInfo GraphicsPipelineComponents::create_viewport_state(VkViewport &viewport, VkRect2D &scissor)
{
    VkPipelineViewportStateCreateInfo result = {};
    result.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    result.viewportCount = 1;
    result.pViewports = &viewport;
    result.scissorCount = 1;
    result.pScissors = &scissor;

    return result;
}

VkPipelineRasterizationStateCreateInfo GraphicsPipelineComponents::create_rasterization_state()
{
    VkPipelineRasterizationStateCreateInfo result = {};
    result.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    result.depthClampEnable = VK_FALSE;
    result.rasterizerDiscardEnable = VK_FALSE;
    result.polygonMode = VK_POLYGON_MODE_FILL;
    result.lineWidth = 1.0f;
    result.cullMode = VK_CULL_MODE_BACK_BIT;
    result.frontFace = VK_FRONT_FACE_CLOCKWISE;
    result.depthBiasEnable = VK_FALSE;

    return result;
}

VkPipelineMultisampleStateCreateInfo GraphicsPipelineComponents::create_multisample_state()
{
    VkPipelineMultisampleStateCreateInfo result = {};
    result.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    result.sampleShadingEnable = VK_FALSE;
    result.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    return result;
}

VkPipelineColorBlendStateCreateInfo GraphicsPipelineComponents::create_color_blend_state()
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

VkPipelineDynamicStateCreateInfo GraphicsPipelineComponents::create_dynamic_state()
{
    VkPipelineDynamicStateCreateInfo result = {};
    result.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    result.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
    result.pDynamicStates = dynamic_states.data();

    return result;
}