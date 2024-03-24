#include "texture_image.h"

#include "stb_image.h"

#include "spdlog/spdlog.h"

using namespace levin;

TextureImage::TextureImage(
    const Device &device,
    const Sampler &sampler,
    const AdhocQueues &adhoc_queues,
    const std::string &name):
    m_device(device),
    m_image_info(create_image_info(name)),
    m_allocation_info(create_allocation_info()),
    m_image_view(create_image_view(adhoc_queues)),
    m_image_descriptor_info(create_image_descriptor_info(sampler))
{
    VkDescriptorImageInfo image_info = {};
    image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    image_info.imageView = m_image_view;
    image_info.sampler = sampler;
}

TextureImage::~TextureImage()
{
    spdlog::info("Destroying image: {}", m_image_info.name);

    vkDestroyImageView(m_device, m_image_view, nullptr);

    vmaDestroyImage(
        m_device.allocator(),
        m_allocation_info.image,
        m_allocation_info.allocation);
}

TextureImage::ImageInfo TextureImage::create_image_info(const std::string &name)
{
    int width, height, channels;
    auto file_name = "assets/textures/" + name;

    spdlog::info("Loading image: {}", file_name);
    auto pixels = stbi_load(file_name.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (!pixels)
    {
        throw std::runtime_error("Failed to load image " + file_name + ": " + stbi_failure_reason());
    }

    VkDeviceSize image_size = width * height * 4;
    auto staging_buffer = std::make_unique<BufferHost>(
        m_device,
        image_size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

    staging_buffer->copy_from(pixels, image_size);

    stbi_image_free(pixels);

    return {
        std::move(staging_buffer),
        name,
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };
}

TextureImage::AllocationInfo TextureImage::create_allocation_info()
{
    spdlog::info("Creating image allocation for: {}", m_image_info.name);

    VkImageCreateInfo image_info = {};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.extent.width = m_image_info.width;
    image_info.extent.height = m_image_info.height;
    image_info.extent.depth = 1;
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.format = VK_FORMAT_R8G8B8A8_SRGB;
    image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_info.flags = 0;

    VmaAllocationCreateInfo image_alloc_info = {};
    image_alloc_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;

    VkImage image;
    VmaAllocation allocation;
    VmaAllocationInfo allocation_info;
    vmaCreateImage(
        m_device.allocator(),
        &image_info,
        &image_alloc_info,
        &image,
        &allocation,
        &allocation_info);

    return {
        image,
        allocation,
        allocation_info,
    };
}

void TextureImage::transition_image_layout(
    const AdhocQueues &adhoc_queues,
    VkImageLayout old_layout,
    VkImageLayout new_layout)
{
    VkCommandBuffer command_buffer = adhoc_queues.graphics().begin();

    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = old_layout;
    barrier.newLayout = new_layout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = m_allocation_info.image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags source_stage;
    VkPipelineStageFlags destination_stage;

    if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
    {
        throw std::invalid_argument("Unsupported layout transition");
    }

    vkCmdPipelineBarrier(
        command_buffer,
        source_stage,
        destination_stage,
        0,
        0,
        nullptr,
        0,
        nullptr,
        1,
        &barrier);

    adhoc_queues.graphics().submit_and_wait();
}

void TextureImage::copy_staging_buffer_to_image(const AdhocQueues &adhoc_queues)
{
    VkCommandBuffer command_buffer = adhoc_queues.transfer().begin();

    VkImageSubresourceLayers subresource = {};
    subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresource.mipLevel = 0;
    subresource.baseArrayLayer = 0;
    subresource.layerCount = 1;

    VkOffset3D offset = {};
    offset.x = 0;
    offset.y = 0;
    offset.z = 0;

    VkExtent3D extent = {};
    extent.width = m_image_info.width;
    extent.height = m_image_info.height;
    extent.depth = 1;

    VkBufferImageCopy region = {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource = subresource;
    region.imageOffset = offset;
    region.imageExtent = extent;

    vkCmdCopyBufferToImage(
        command_buffer,
        *m_image_info.staging_buffer,
        m_allocation_info.image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region);

    adhoc_queues.transfer().submit_and_wait();
}

VkImageView TextureImage::create_image_view(const AdhocQueues &adhoc_queues)
{
    spdlog::info("Creating image view for: {}", m_image_info.name);

    transition_image_layout(
        adhoc_queues,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    copy_staging_buffer_to_image(adhoc_queues);

    transition_image_layout(
        adhoc_queues,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    m_image_info.staging_buffer.reset();

    VkImageViewCreateInfo view_info = {};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.image = m_allocation_info.image;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = VK_FORMAT_R8G8B8A8_SRGB;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;

    VkImageView view;
    if (vkCreateImageView(m_device, &view_info, nullptr, &view) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create image view");
    }

    return view;
}

VkDescriptorImageInfo TextureImage::create_image_descriptor_info(const Sampler &sampler)
{
    VkDescriptorImageInfo image_info = {};
    image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    image_info.imageView = m_image_view;
    image_info.sampler = sampler;

    return image_info;
}