#include "sfpch.hpp"
#include "Instance.hpp"

#include "utility/log/Log.hpp"
#include "graphics/vulkan/utils/ValidationLayers.hpp"
#include "graphics/vulkan/utils/Debug.hpp"
#include "graphics/vulkan/utils/Extensions.hpp"

#include <GLFW/glfw3.h>

#include <sstream>

// #TODO-Zeth : Create necessary enum class for replace enum C style of Vulkan API for Instance class
// #TODO-Zeth : Think about parameters for create the instance

namespace Silfur
{
    namespace Vk
    {
        void Instance::Create(const std::string& p_appName, const Version& p_appVersion)
        {
            if (enableValidationLayers && !checkValidationLayerSupport())
            {
                SF_CORE_FATAL(Vulkan, 20, "Validation layers requested but not available!");
                std::exit(EXIT_FAILURE);
            }

            VkApplicationInfo appInfo = {};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = p_appName.c_str();
            appInfo.applicationVersion = VK_MAKE_VERSION(p_appVersion.major, p_appVersion.minor, p_appVersion.patch);
            appInfo.pEngineName = "Silfur Engine";
            appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);

            // #NOTE : vkEnumerateInstanceVersion function exists since Vulkan 1.1
            auto func = (PFN_vkEnumerateInstanceVersion) vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion");
            if (func != nullptr)
            {
                uint32_t supportedApiVersion;
                vkEnumerateInstanceVersion(&supportedApiVersion);

                appInfo.apiVersion = supportedApiVersion;
            }
            else
            {
                appInfo.apiVersion = VK_API_VERSION_1_0;
            }

            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
            auto requiredExtensions = getRequiredExtensions();
            VkInstanceCreateInfo createInfo = createVkInstanceCreateInfo(appInfo, requiredExtensions, debugCreateInfo);

            if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
            {
                SF_CORE_FATAL(Vulkan, 22, "Failed to create instance!");
                std::exit(EXIT_FAILURE);
            }

            SF_CORE_INFO(Vulkan, "Create vulkan instance for application: {}, version {}.{}.{}", appInfo.pApplicationName,
                VK_VERSION_MAJOR(appInfo.applicationVersion), VK_VERSION_MINOR(appInfo.applicationVersion),
                VK_VERSION_PATCH(appInfo.applicationVersion));

            setupDebugMessenger();
        }

        void Instance::Destroy()
        {
            if (enableValidationLayers)
            {
                DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
            }

            vkDestroyInstance(m_Instance, nullptr);
        }

        VkSurfaceKHR Instance::CreateSurface(const Window& p_window)
        {
            VkSurfaceKHR surface;

            if (glfwCreateWindowSurface(m_Instance, p_window, nullptr, &surface) != VK_SUCCESS)
            {
                SF_CORE_FATAL(Vulkan, 24, "Failed to create window surface!");
                std::exit(EXIT_FAILURE);
            }

            return surface;
        }

        VkInstanceCreateInfo Instance::createVkInstanceCreateInfo(const VkApplicationInfo& p_appInfo,
            const std::vector<const char*>& p_requiredExtensions,
            VkDebugUtilsMessengerCreateInfoEXT& p_debugCreateInfo)
        {
            VkInstanceCreateInfo createInfo;
            createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &p_appInfo;

            uint32_t vkExtensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, nullptr);
            std::vector<VkExtensionProperties> vkExtensions(vkExtensionCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, vkExtensions.data());

            std::stringstream msg;
            msg << "Vulkan available extensions:\n";
            for (const auto& extension : vkExtensions)
            {
                msg << "\t" << extension.extensionName << "\n";
            }
            SF_CORE_INFO(Vulkan, msg.str());

            msg.str("");
            msg.clear();
            msg << "Required extensions:\n";
            for (const char* extensionName : p_requiredExtensions)
            {
                msg << "\t" << extensionName << "\n";
            }
            SF_CORE_INFO(Vulkan, msg.str());

            if (!checkRequiredExtensions(vkExtensions, p_requiredExtensions))
            {
                SF_CORE_FATAL(Vulkan, 21, "Missing an extension.");
                std::exit(EXIT_FAILURE);
            }

            createInfo.enabledExtensionCount = static_cast<uint32_t>(p_requiredExtensions.size());
            createInfo.ppEnabledExtensionNames = p_requiredExtensions.data();

            
            if (enableValidationLayers)
            {
                createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
                createInfo.ppEnabledLayerNames = validationLayers.data();

                populateDebugMessengerCreateInfo(p_debugCreateInfo);
                createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &p_debugCreateInfo;
            }
            else
            {
                createInfo.enabledLayerCount = 0;
                createInfo.pNext = nullptr;
            }

            return createInfo;
        }

        void Instance::setupDebugMessenger()
        {
            if (!enableValidationLayers) return;

            VkDebugUtilsMessengerCreateInfoEXT createInfo;
            populateDebugMessengerCreateInfo(createInfo);

            if (CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
            {
                SF_CORE_FATAL(Vulkan, 23, "Failed to set up debug messenger!");
                std::exit(EXIT_FAILURE);
            }
        }
    }
}
