#pragma once

#ifndef __SILFUR_CORE_WINDOW_WINDOW_HPP__
#define __SILFUR_CORE_WINDOW_WINDOW_HPP__

#include "core/Base.hpp"
#include "VideoMode.hpp"

#include <SDL2/SDL_events.h>

struct SDL_Window;

namespace Silfur
{
    class Window;
    using WindowPtr = Scope<Window>;

    class Window
    {
    public:
        Window() = delete;
        Window(VideoMode p_mode, const char* p_title);
        ~Window();

        void ProcessEvents();
        void Shutdown();

        inline operator SDL_Window*() const noexcept;
        inline SDL_Window* WindowHandle() const noexcept;
        void* WindowSystemHandle() const noexcept;

    private:
        void Create(VideoMode p_mode, const char* p_title);
        int static SDLCALL HandleEvent(void* p_userdata, SDL_Event* p_event);

    public:
        bool IsClosed = false;

    private:
        SDL_Window* m_WinHandle {};
    };
}

#include "Window.inl"

#endif // __SILFUR_CORE_WINDOW_WINDOW_HPP__
