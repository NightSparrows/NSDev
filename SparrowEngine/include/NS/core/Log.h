#pragma once

#include "Base.h"

#pragma warning(push, 0)
#include "deps/spdlog/spdlog/spdlog.h"
#include "deps/spdlog/spdlog/fmt/ostr.h"
#pragma warning(pop)

namespace ns {

	class Log {
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;

	public:
		static void Init();
		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	};

}


#define NS_CORE_TRACE(...) ::ns::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define NS_CORE_INFO(...) ::ns::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NS_CORE_WARN(...) ::ns::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NS_CORE_ERROR(...) ::ns::Log::GetCoreLogger()->error(__VA_ARGS__)
#define NS_CORE_CRITICAL(...) ::ns::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define NS_TRACE(...) ::ns::Log::GetClientLogger()->trace(__VA_ARGS__)
#define NS_INFO(...) ::ns::Log::GetClientLogger()->info(__VA_ARGS__)
#define NS_WARN(...) ::ns::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NS_ERROR(...) ::ns::Log::GetClientLogger()->error(__VA_ARGS__)
#define NS_CRITICAL(...)      ::ns::Log::GetClientLogger()->critical(__VA_ARGS__)

#ifdef NS_ENABLE_ASSERTS

#define NS_CORE_ASSERT(x, ...) if (!x) { NS_CORE_CRITICAL(__VA_ARGS__); NS_DEBUGBREAK(); }
#define NS_ASSERT(x, ...) if (!x) { NS_CRITICAL(__VA_ARGS__); NS_DEBUGBREAK(); }

#else
#define NS_ASSERT(...)
#define NS_CORE_ASSERT(...)
#endif