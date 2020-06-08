#pragma once

#ifndef __SILFUR_GRAPHICS_VULKAN_INSTANCE_HPP__
#define __SILFUR_GRAPHICS_VULKAN_INSTANCE_HPP__

#include "core/Window.hpp"

#include <vulkan/vulkan.h>

#include <vector>
#include <string>

// #TODO-Zeth : Handle physical device needs

namespace Silfur
{
    namespace Vk
    {
        class Instance
        {
        public:
            Instance() = default;
            ~Instance() = default;

            Instance(const Instance&) = delete;
            Instance(Instance&&) = delete;

            void Create(const std::string& p_appName, const Version& p_appVersion);
            void Destroy();

            VkSurfaceKHR CreateSurface(const Window& p_window);

            Instance& operator=(const Instance&) = delete;
            Instance& operator=(Instance&&) = delete;

            inline operator VkInstance() const noexcept;
        private:
            VkInstanceCreateInfo createVkInstanceCreateInfo(const VkApplicationInfo& p_appInfo,
                const std::vector<const char*>& p_requiredExtensions,
                VkDebugUtilsMessengerCreateInfoEXT& p_debugCreateInfo);
            void setupDebugMessenger();

        private:
            VkInstance m_Instance {};
            VkDebugUtilsMessengerEXT m_DebugMessenger {};
        };
    }
}

#include "graphics/vulkan/Instance.inl"

#endif // __SILFUR_GRAPHICS_VULKAN_INSTANCE_HPP__