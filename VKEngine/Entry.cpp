#include <iostream>
#include "gvkInitUtils.h"

int main(int argc, char ** argv) {
	VkInstance testInst = VK_NULL_HANDLE;
	VkDebugReportCallbackEXT callback = VK_NULL_HANDLE;
	gvkInitUtils::initVulkan(&testInst);
	gvkInitUtils::registerDefaultDebugCallback(testInst, &callback);

	std::cin.get();
}