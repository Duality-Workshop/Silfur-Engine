#include "sfpch.h"
#include "Instance.h"

#include "utility/log/Log.h"
#include "graphics/vulkan/ValidationLayers.h"

// #TODO (Zeth) : Check api version available on the computer, fallback on version 1.0
// #TODO (Zeth) : Replace exception by logging (fatal level) + std::exit
// #TODO (Zeth) : Replace std::cout and std::cerr by logging (std::cout = information | std::cerr = warning/error)
// #TODO (Zeth) : Create necessary enum class for replace enum C style of Vulkan API for Instance class

namespace Silfur
{
    namespace Vk
    {
        VkResult CreateDebugUtilsMessengerEXT(VkInstance p_instance,
            const VkDebugUtilsMessengerCreateInfoEXT* p_pCreateInfo,
            const VkAllocationCallbacks* p_pAllocator,
            VkDebugUtilsMessengerEXT* p_pDebugMessenger)
        {
            auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(p_instance, "vkCreateDebugUtilsMessengerEXT");
            if (func != nullptr)
            {
                return func(p_instance, p_pCreateInfo, p_pAllocator, p_pDebugMessenger);
            }
            else
            {
                return VK_ERROR_EXTENSION_NOT_PRESENT;
            }
        }

        void DestroyDebugUtilsMessengerEXT(VkInstance p_instance,
            VkDebugUtilsMessengerEXT p_debugMessenger,
            const VkAllocationCallbacks* p_pAllocator)
        {
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(p_instance, "vkDestroyDebugUtilsMessengerEXT");
            if (func != nullptr)
            {
                func(p_instance, p_debugMessenger, p_pAllocator);
            }
        }

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT p_MessageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT p_MessageType,
            const VkDebugUtilsMessengerCallbackDataEXT* p_pCallbackData,
            void* p_pUserData)
        {
            std::cerr << "Validation layer: " << p_pCallbackData->pMessage << std::endl;

            return VK_FALSE;
        }

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& p_createInfo)
        {
            p_createInfo = {};
            p_createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            p_createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            p_createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            p_createInfo.pfnUserCallback = debugCallback;
            p_createInfo.pUserData = nullptr; // Optional
        }

        bool checkValidationLayerSupport()
        {
            uint32_t layerCount;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

            std::vector<VkLayerProperties> availableLayers(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

            for (const char* layerName : validationLayers)
            {
                bool layerFound = false;

                for (const auto& layerProperties : availableLayers)
                {
                    if (strcmp(layerName, layerProperties.layerName) == 0)
                    {
                        layerFound = true;
                        break;
                    }
                }

                if (!layerFound)
                {
                    return false;
                }
            }

            std::cout << "Validation layers enabled." << std::endl;

            return true;
        }

        std::vector<const char*> getRequiredExtensions()
        {
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

            if (enableValidationLayers)
            {
                extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }

            return extensions;
        }

        bool checkRequiredExtensions(const std::vector<VkExtensionProperties>& p_VkExtensionsAvailable, const std::vector<const char*> p_RequiredExtensions)
        {
            for (const char* extensionName : p_RequiredExtensions)
            {
                bool extensionFound = false;

                for (const auto& vkExtension : p_VkExtensionsAvailable)
                {
                    if (strcmp(extensionName, vkExtension.extensionName) == 0)
                    {
                        extensionFound = true;
                        break;
                    }
                }

                if (!extensionFound)
                {
                    return false;
                }
            }

            std::cout << "Extensions requirements fulfilled." << std::endl;

            return true;
        }

        void Instance::Create()
        {
            if (enableValidationLayers && !checkValidationLayerSupport())
            {
                throw std::runtime_error("Validation layers requested but not available!");
            }

            VkApplicationInfo appInfo = {};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = "Hello Triangle";
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.pEngineName = "Silfur Engine";
            appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);

            appInfo.apiVersion = VK_API_VERSION_1_2;

            VkInstanceCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;

            uint32_t vkExtensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, nullptr);
            std::vector<VkExtensionProperties> vkExtensions(vkExtensionCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, vkExtensions.data());

            std::cout << "Vulkan available extensions:" << std::endl;
            for (const auto& extension : vkExtensions)
            {
                std::cout << "\t" << extension.extensionName << std::endl;
            }

            auto extensions = getRequiredExtensions();

            std::cout << "Required extensions:" << std::endl;
            for (const char* extensionName : extensions)
            {
                std::cout << "\t" << extensionName << std::endl;
            }

            if (!checkRequiredExtensions(vkExtensions, extensions))
            {
                throw std::runtime_error("Missing an extension.");
            }

            createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
            createInfo.ppEnabledExtensionNames = extensions.data();

            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
            if (enableValidationLayers)
            {
                createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
                createInfo.ppEnabledLayerNames = validationLayers.data();

                populateDebugMessengerCreateInfo(debugCreateInfo);
                createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
            }
            else
            {
                createInfo.enabledLayerCount = 0;
                createInfo.pNext = nullptr;
            }

            if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create instance!");
            }

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

            if (glfwCreateWindowSurface(m_Instance, p_window.WinHandle, nullptr, &surface) != VK_SUCCESS)
            {
                SF_CORE_FATAL(Vulkan, 25, "Failed to create window surface!");
                std::exit(EXIT_FAILURE);
            }

            return surface;
        }

        void Instance::setupDebugMessenger()
        {
            if (!enableValidationLayers) return;

            VkDebugUtilsMessengerCreateInfoEXT createInfo;
            populateDebugMessengerCreateInfo(createInfo);

            if (CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to set up debug messenger!");
            }
        }
    }
}