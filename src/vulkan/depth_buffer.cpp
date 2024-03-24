#include "depth_buffer.h"

#include "spdlog/spdlog.h"

using namespace levin;

DepthBuffer::DepthBuffer(
    const Device &device,
    const Swapchain &swapchain) :
      m_image(
        device,
        swapchain.extent().width,
        swapchain.extent().height,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_IMAGE_TILING_OPTIMAL),
      m_image_view(device, m_image, VK_IMAGE_ASPECT_DEPTH_BIT)
{
}