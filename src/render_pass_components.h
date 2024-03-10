#pragma once

#include <vector>

#include "device_components.h"

#include "render_pass_factory.h"

namespace levin
{
    class RenderPassComponents
    {
    private:
        RenderPassFactory m_factory;

        VkRenderPass m_render_pass;

        static VkRenderPass create_render_pass(RenderPassFactory &factory);

    public:
        RenderPassComponents(const DeviceComponents &device_components);
        RenderPassComponents(const RenderPassComponents &) = delete;

        VkRenderPass get_render_pass() const { return m_render_pass; }
    };
}