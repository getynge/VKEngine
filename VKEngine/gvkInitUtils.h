#pragma once
#include <cstdint>
#include <vulkan\vulkan.h>
namespace gvkInitUtils {
	VkResult initVulkan(VkInstance * inst);
	VkResult registerDefaultDebugCallback(VkInstance, VkDebugReportCallbackEXT *);
	VKAPI_ATTR VkBool32 VKAPI_CALL defaultDebugReportCallback(
		VkDebugReportFlagsEXT,
		VkDebugReportObjectTypeEXT,
		uint64_t,
		size_t,
		int32_t,
		const char*,
		const char*,
		void*
	);


}