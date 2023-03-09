#include <nspch.h>
#include <NS/core/io/Window.h>

#ifdef NS_PLATFORM_WINDOWS

#include <platform/Windows/WindowsWindow.h>

#endif // NS_PLATFORM_WINDOWS


namespace ns {


	Scope<Window> Window::Create(const WindowProperties &properties) {
#ifdef NS_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(properties);
#else
		return nullptr;
#endif
	}


}
