#ifdef _WIN32
#ifdef DLL_EXPORT
#define API_PREFIX __declspec(dllexport)
#else
#define API_PREFIX __declspec(dllimport)
#endif

#else
#define API_PREFIX
#endif
