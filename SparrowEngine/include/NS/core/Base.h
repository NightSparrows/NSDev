#pragma once

#include <string>
#include <memory>
#include <vector>

#include "PlatformDetection.h"

// TODO: Deal with dynamic linking
#define NS_API

#ifdef NS_DEBUG
	#if defined(NS_PLATFORM_WINDOWS)
		#define NS_DEBUGBREAK() __debugbreak()
	#elif defined(NS_PLATFORM_LINUX)
		#include <signal.h>
		#define NS_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define NS_ENABLE_ASSERTS
#else
	#define NS_DEBUGBREAK()
#endif

namespace ns {

	// �ߤ@�A�H�����
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	// �ѦҡA����const CLASS &a = b
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	// All "common" platforms use the same size for char, short and int
	// (basically there are 3 types for 3 sizes, so no other match is possible),
	// we can use them without doing any kind of check

	// 8 bits integer types
	typedef int8_t Int8;
	typedef uint8_t Uint8;

	// 16 bits integer types
	typedef int16_t Int16;
	typedef uint16_t Uint16;

	// 32 bits integer types
	typedef int32_t Int32;
	typedef uint32_t Uint32;

	// 64 bits integer types
#if defined(_MSC_VER)
	typedef signed   __int64 Int64;
	typedef unsigned __int64 Uint64;
#else
	typedef signed   long long Int64;
	typedef unsigned long long Uint64;
#endif

}
