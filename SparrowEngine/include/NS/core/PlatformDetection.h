// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		#define NS_PLATFORM_WINDOWS
	#else
		#error "x86 builds aren't supported."
	#endif // _WIN64
#else
	#error "Platform not supported."
#endif // _WIN32


