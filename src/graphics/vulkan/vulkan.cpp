#include "pch.h"
#include "exceptions/exceptions.h"
#include "vulkan.h"
#include "pipelines/vk_sprite_pipeline.h"
#include "renderpasses/vk_renderpasses.h"
#include "vk_draw.h"
#include "pipelines/vk_tiles_pipeline.h"
#include <graphics/vulkan/descriptor_sets/vk_atlas_descriptor_set.h>
#include <console/console.h>
#include "system_objects/vk_instance.h"
#include "system_objects/vk_physicaldevice.h"
#include "system_objects/vk_queuefamilies.h"
#include "system_objects/vk_device.h"
#include "system_objects/vk_queues.h"
#include "system_objects/vk_surface.h"
#include "system_objects/vk_commandbuffers.h"
#include "system_objects/vk_sync.h"
#include "system_objects/vk_swapchain.h"
#include "game_objects/vk_vertexbuffer.h"
#include "game_objects/vk_renderimage.h"
#include "game_objects/vk_objects_instancebuffer.h"
#include "game_objects/vk_atlas_texture.h"
#include "game_objects/vk_point_sampler.h"
#include "game_objects/vk_level_texture.h"
#include "descriptor_sets/vk_level_descriptor_set.h"
#include "descriptor_sets/vk_static_descriptor_pool.h"
#include "game_objects/vk_win_screen.h"

using namespace Console;

namespace Graphics::Vulkan
{
	static uint32_t nextImageIndex = 0;

	void init()
	{
#ifdef DEBUG
		if (!CompileShaders())
		{
			log("Error compiling shaders");
			throw runtime_error("Shader compilation error");
		}
#endif // DEBUG

		create_instance();
		pick_physical_device();
		get_queue_families();
		get_memory_types();
		create_device();
		get_device_queues();
		create_surface();
		create_command_buffers();
		create_sync_objects();
		get_swapchain_format();
		create_render_passes();
		create_swapchain();
		create_point_sampler();

		create_vertex_buffer();
		create_objects_buffer();
		create_atlas_texture();
		create_render_image();
		create_level_image();
		create_win_texture();

		allocate_static_memory();

		load_atlas_texture();
		load_win_texture();
		load_vertex_buffer();
		create_render_image_view();
		create_level_image_view();

		create_descriptor_pool();

		create_atlas_descriptor_set();
		create_level_descriptor_set();
		create_sprite_pipeline();
		create_tile_pipeline();
	}

	void shutdown()
	{
		vk_assert(vkDeviceWaitIdle(vk_device));

		destroy_descriptor_pool();
		destroy_level_descriptor_set();
		destroy_level_image();
		destroy_win_texture();
		destroy_point_sampler();
		destroy_object_buffer();
		destroy_render_image();
		destroy_atlas_descriptor_set();
		destroy_atlas_texture();
		destroy_tiles_pipeline();
		destroy_sprite_pipeline();
		destroy_vertex_buffer();
		destroy_render_passes();
		destroy_swapchain();
		destroy_sync_objects();
		destroy_command_buffer();
		destroy_surface();

		free_static_memory();

		destroy_device();
		destroy_instance();
	}

	void wait_for_frame()
	{
		vk_assert(vkWaitForFences(vk_device, 1, &vk_fence_main, VK_TRUE, UINT64_MAX));
		vkResetFences(vk_device, 1, &vk_fence_main);

		VkResult result;
		while (true)
		{
			result = vkAcquireNextImageKHR(vk_device, vk_swapchain, UINT64_MAX, vk_semaphore_acquireimage, VK_NULL_HANDLE, &nextImageIndex);
			if (result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				recreate_swapchain();
				continue;
			}

			break;
		}
	}

	void render_frame()
	{
		draw_frame(nextImageIndex);

		VkPresentInfoKHR present{
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &vk_semaphore_rendering,
			.swapchainCount = 1,
			.pSwapchains = &vk_swapchain,
			.pImageIndices = &nextImageIndex,
		};

		VkResult result = vkQueuePresentKHR(vk_queue_main, &present);
		if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			vk_assert(vkWaitForFences(vk_device, 1, &vk_fence_main, VK_TRUE, UINT64_MAX));
			recreate_swapchain();
		}
	}

	void vk_assert(VkResult result)
	{
		if (result < 0)
		{
			throw VulkanException("Vulkan Error: ", result);
		}
	}
}