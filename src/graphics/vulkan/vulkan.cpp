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

namespace Graphics::Vulkan
{
	static uint32_t nextImageIndex = 0;

	void Init()
	{
#ifdef DEBUG
		if (!CompileShaders())
		{
			Log("Error compiling shaders");
			throw runtime_error("Shader compilation error");
		}
#endif // DEBUG

		CreateInstance();
		PickPhysicalDevice();
		GetQueueFamilies();
		GetMemoryTypes();
		CreateDevice();
		GetDeviceQueues();
		CreateSurface();
		CreateCommandBuffers();
		CreateSyncObjects();
		GetSwapchainFormat();
		CreateRenderPasses();
		CreateSwapchain();
		CreatePointSampler();

		CreateVertexBuffer();
		CreateObjectsBuffer();
		CreateAtlasTexture();
		CreateRenderImage();
		CreateLevelImage();
		CreateWinTexture();

		AllocateStaticMemory();

		LoadAtlasTexture();
		LoadWinTexture();
		LoadVertexBuffer();
		CreateRenderImageView();
		CreateLevelImageView();

		CreateDescriptorPool();

		CreateAtlasDescriptorSet();
		CreateLevelDescriptorSet();
		CreateSpritePipeline();
		CreateTilesPipeline();
	}

	void Shutdown()
	{
		VkAssert(vkDeviceWaitIdle(vk_device));

		DestroyDescriptorPool();
		DestroyLevelDescriptorSet();
		DestroyLevelImage();
		DestroyWinTexture();
		DestroyPointSampler();
		DestroyObjectsBuffer();
		DestroyRenderImage();
		DestroyAtlasDescriptorSet();
		DestroyAtlasTexture();
		DestroyTilesPipeline();
		DestroySpritePipeline();
		DestroyVertexBuffer();
		DestroyRenderPasses();
		DestroySwapchain();
		DestroySyncObjects();
		DestroyCommandBuffers();
		DestroySurface();

		FreeStaticMemory();

		DestroyDevice();
		DestroyInstance();
	}

	void WaitForFrame()
	{
		VkAssert(vkWaitForFences(vk_device, 1, &vk_fence_main, VK_TRUE, UINT64_MAX));
		vkResetFences(vk_device, 1, &vk_fence_main);

		VkResult result;
		while (true)
		{
			result = vkAcquireNextImageKHR(vk_device, vk_swapchain, UINT64_MAX, vk_semaphore_acquireimage, VK_NULL_HANDLE, &nextImageIndex);
			if (result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				RecreateSwapchain();
				continue;
			}

			break;
		}
	}

	void RenderFrame()
	{
		DrawFrame(nextImageIndex);

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
			VkAssert(vkWaitForFences(vk_device, 1, &vk_fence_main, VK_TRUE, UINT64_MAX));
			RecreateSwapchain();
		}
	}

	void VkAssert(VkResult result)
	{
		if (result < 0)
		{
			throw VulkanException("Vulkan Error: ", result);
		}
	}
}