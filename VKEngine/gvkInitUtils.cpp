#include "gvkInitUtils.h"
#include <vector>
#include <array>
#include <iostream>

bool hasLayer(const char * name) {
	uint32_t count = 0;
	std::vector<VkLayerProperties> layerprops;
	vkEnumerateInstanceLayerProperties(&count, nullptr);
	if (count != 0) {
		layerprops = std::vector<VkLayerProperties>(count);
		vkEnumerateInstanceLayerProperties(&count, layerprops.data());
		for (auto prop : layerprops) {
			if (strcmp(name, prop.layerName) == 0) {
				std::cout << "found layer: " << prop.layerName << std::endl;
				return true;
			}
		}
	} 
	return false;
}

bool hasExtension(const char * name, const char * layerName) {
	uint32_t count = 0;
	std::vector<VkExtensionProperties> extprops;
	vkEnumerateInstanceExtensionProperties(layerName, &count, nullptr);
	if (count == 0) return false;
	extprops = std::vector<VkExtensionProperties>(count);
	vkEnumerateInstanceExtensionProperties(layerName, &count, extprops.data());
	for (auto prop : extprops) {
		if (strcmp(name, prop.extensionName) == 0) {
			std::cout << "found extension: " << prop.extensionName << std::endl;
			return true;
		}
	}
	return false;
}

//Creates our vulkan instance and returns an error if it fails
VkResult gvkInitUtils::initVulkan(VkInstance * inst)
{
	VkApplicationInfo appinfo = {};
	appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appinfo.pNext = nullptr;
	appinfo.pApplicationName = "TODO: CHANGE THIS";
	appinfo.pEngineName = "VKEngine";
	appinfo.apiVersion = VK_API_VERSION_1_0;
	appinfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appinfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

	std::vector<const char *> enableExtensions = { VK_KHR_SURFACE_EXTENSION_NAME};

#ifdef _WIN32
	enableExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(__linux__)
	enableExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif
	
	VkInstanceCreateInfo instCrInfo = {};
	instCrInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instCrInfo.pApplicationInfo = &appinfo;
	instCrInfo.pNext = nullptr;
#ifdef _DEBUG
	std::array<const char*, 1> enabledLayers = {"VK_LAYER_LUNARG_standard_validation" };
	if (!hasLayer("VK_LAYER_LUNARG_standard_validation")) {
		std::cerr << "ERROR REQUIRED LAYER DOES NOT EXIST" << std::endl;
		return VK_ERROR_INCOMPATIBLE_DRIVER;
	}
	enableExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	instCrInfo.enabledLayerCount = enabledLayers.size();
	instCrInfo.ppEnabledLayerNames = enabledLayers.data();
	if (!hasExtension(VK_EXT_DEBUG_REPORT_EXTENSION_NAME, "VK_LAYER_LUNARG_standard_validation")) {
		std::cerr << "ERROR: REQUIRED EXTENSION DOES NOT EXIST" << std::endl;
		return VK_ERROR_INCOMPATIBLE_DRIVER;
	}
	
#else
	instCrInfo.enabledLayerCount = 0;
	instCrInfo.ppEnabledLayerNames = nullptr;
#endif
	instCrInfo.flags = 0;

	instCrInfo.enabledExtensionCount = enableExtensions.size();
	instCrInfo.ppEnabledExtensionNames = enableExtensions.data();
	return vkCreateInstance(&instCrInfo, nullptr, inst);
}


VkResult gvkInitUtils::registerDefaultDebugCallback(VkInstance inst, VkDebugReportCallbackEXT * callback)
{
	using dbgext = PFN_vkCreateDebugReportCallbackEXT;
	dbgext CreateDebugReportCallback = VK_NULL_HANDLE;
	CreateDebugReportCallback = (dbgext) vkGetInstanceProcAddr(inst, "vkCreateDebugReportCallbackEXT");
	
	VkDebugReportCallbackCreateInfoEXT callbackCreateInfo = {};
	callbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	callbackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT |
		VK_DEBUG_REPORT_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
	callbackCreateInfo.pfnCallback = &defaultDebugReportCallback;

	return CreateDebugReportCallback(inst, &callbackCreateInfo, nullptr, callback);
}

VKAPI_ATTR VkBool32 VKAPI_CALL gvkInitUtils::defaultDebugReportCallback(
	VkDebugReportFlagsEXT, 
	VkDebugReportObjectTypeEXT,
	uint64_t, 
	size_t, 
	int32_t,
	const char *, 
	const char * pMessage, //we ignore every argument other than this for now, maybe we use these later
	void *)
{
	std::cerr << pMessage << std::endl;
	return VK_FALSE;
}
