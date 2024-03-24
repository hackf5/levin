#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

#include "util/no_copy_or_move.h"
#include "buffer/buffer_host.h"
#include "adhoc_queues.h"
#include "device.h"
#include "sampler.h"
#include "swapchain.h"
#include "descriptor_set_layout.h"
#include "graphics_pipeline.h"

namespace levin
{
    class Image: public NoCopyOrMove
    {
    private:
        struct AllocationInfo
        {
            const VkImage image;
            const VmaAllocation allocation;
            const VmaAllocationInfo info;
        };

        struct ImageInfo
        {
            std::unique_ptr<BufferHost> staging_buffer;
            const std::string name;
            const uint32_t width;
            const uint32_t height;
        };

        const Device &m_device;

        ImageInfo m_image_info;
        const AllocationInfo m_allocation_info;
        const VkImageView m_image_view;
        VkDescriptorImageInfo m_image_descriptor_info;

        ImageInfo create_image_info(const std::string &name);
        AllocationInfo create_allocation_info();
        VkImageView create_image_view(const AdhocQueues &adhoc_queues);
        VkDescriptorImageInfo create_image_descriptor_info(const Sampler &sampler);

        void transition_image_layout(
            const AdhocQueues &adhoc_queues,
            VkImageLayout old_layout,
            VkImageLayout new_layout);

        void copy_staging_buffer_to_image(const AdhocQueues &adhoc_queues);

    public:
        Image(
            const Device &device,
            const Sampler &sampler,
            const AdhocQueues &adhoc_queues,
            const std::string &name);
        ~Image();

        void bind(GraphicsPipeline &pipeline)
        {
            pipeline
                .descriptor_set_layout()
                .write_combined_image_sampler(&m_image_descriptor_info, 2);
        }
    };
}