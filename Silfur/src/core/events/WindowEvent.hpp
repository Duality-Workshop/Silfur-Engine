#pragma once

#ifndef __SILFUR_CORE_EVENTS_WINDOW_EVENT_HPP__
#define __SILFUR_CORE_EVENTS_WINDOW_EVENT_HPP__

#include "Event.hpp"

namespace Silfur
{
    typedef struct WindowEventInfo
    {
        union
        {
            int32_t x;
            int32_t width;
        };

        union
        {
            int32_t y;
            int32_t height;
        };
    } WindowEventInfo;

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowCloseEvent triggered";
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowClose)
    };

    class WindowMovedEvent : public Event
    {
    public:
        explicit WindowMovedEvent(WindowEventInfo p_windowEventInfo)
            : m_WindowEventInfo(p_windowEventInfo)
        {}

        inline WindowEventInfo GetInfos() const { return m_WindowEventInfo; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowMovedEvent. Window moved on: X: " << m_WindowEventInfo.x << " | Y: "
            << m_WindowEventInfo.y;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowMoved)
    private:
        WindowEventInfo m_WindowEventInfo;
    };

    class WindowSizeChangedEvent : public Event
    {
    public:
        explicit WindowSizeChangedEvent(WindowEventInfo p_windowEventInfo)
            : m_WindowEventInfo(p_windowEventInfo)
        {}

        inline WindowEventInfo GetInfos() const { return m_WindowEventInfo; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowSizeChangedEvent. Window size change to: Width: " << m_WindowEventInfo.width
            << " | Height: " << m_WindowEventInfo.height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowSizeChanged)
    private:
        WindowEventInfo m_WindowEventInfo;
    };

    class WindowResizedEvent : public Event
    {
    public:
        explicit WindowResizedEvent(WindowEventInfo p_windowEventInfo)
            : m_WindowEventInfo(p_windowEventInfo)
        {}

        inline WindowEventInfo GetInfos() const { return m_WindowEventInfo; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizedEvent. Window resize to: Width: " << m_WindowEventInfo.width
            << " | Height: " << m_WindowEventInfo.height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResized)
    private:
        WindowEventInfo m_WindowEventInfo;
    };

    class WindowEnterEvent : public Event
    {
    public:
        WindowEnterEvent() = default;

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowEnterEvent triggered";
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowEnter)
    };

    class WindowLeaveEvent : public Event
    {
    public:
        WindowLeaveEvent() = default;

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowLeaveEvent triggered";
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowLeave)
    };

    class WindowFocusGainedEvent : public Event
    {
    public:
        WindowFocusGainedEvent() = default;

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowFocusGainedEvent triggered";
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowFocusGained)
    };

    class WindowFocusLostEvent : public Event
    {
    public:
        WindowFocusLostEvent() = default;

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowFocusLostEvent triggered";
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowFocusLost)
    };
}

#endif // __SILFUR_CORE_EVENTS_WINDOW_EVENT_HPP__