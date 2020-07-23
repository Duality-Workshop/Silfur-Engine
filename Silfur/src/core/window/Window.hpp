#pragma once

#ifndef __SILFUR_CORE_WINDOW_WINDOW_HPP__
#define __SILFUR_CORE_WINDOW_WINDOW_HPP__

#include "core/Core.hpp"
#include "VideoMode.hpp"

struct GLFWwindow;

namespace Silfur
{
    class Window;
    using WindowPtr = Scope<Window>;
    class EventManager;

    class Window
    {
    public:
        Window() = delete;
        Window(VideoMode p_mode, const char* p_title, const EventManager& p_EventManager);
        ~Window();

        void ProcessEvents();

        inline operator GLFWwindow*() const noexcept;
        inline GLFWwindow* WindowHandle() const noexcept;
        inline void* WindowSystemHandle() const noexcept;

    private:
        void Create(VideoMode p_mode, const char* p_title);
        void Shutdown();

    public:
        bool IsClosed = false;

    private:
        GLFWwindow* m_WinHandle {};
    };
}

#include "Window.inl"

#endif // __SILFUR_CORE_WINDOW_WINDOW_HPP__