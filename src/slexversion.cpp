
// version of so/dll-file
#define SL_VERSION "01.01.00.00"

#if defined(WIN32) || defined(WIN64)
#   define SL_VER_EXPORT __declspec(dllexport)
#else
#   define SL_VER_EXPORT
#endif

namespace SL_MODULE
{
    extern const char* const SL_VER_VERSION_INFO = "@(#) $program_version_id:\n"
                             SL_VERSION"\n";
}

extern "C"
{
	// global function
    SL_VER_EXPORT const char* slVerGetVersion(void)
    {
        return SL_MODULE::SL_VER_VERSION_INFO;
    }
}
