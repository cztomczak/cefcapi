cefcapi - CEF C API
===================

A simple example on how to use the C API in [Chromium Embedded
Framework](https://bitbucket.org/chromiumembedded/cef).
Example creates a window, embeds a browser in it and loads
Google website. Shows how to implement dummy reference
counting and CEF structures with callbacks to 
implement CEF handlers like cef_app_t and cef_client_t.


Compatibility
-------------

Examples are available for both Linux and Windows.

Tested with CEF branch 3029:
- Linux: `cef_binary_3.3029.1608.g27a32db_linux64.tar.bz2`
- Windows: example not yet updated for branch 3029


Getting started
---------------
1. Download "Standard Distribution" binaries from Spotify:
    http://opensource.spotify.com/cefbuilds/index.html
2. Copy cef_binary*/Release/* to cefcapi/Release/
3. Copy cef_binary*/Resources/* to cefcapi/Release/
4. On Linux run the "make" command
5. On Windows run the "build.bat" script


GCC compiler on Windows
-----------------------
It was required to modify cef_export.h to make it work with GCC
compiler on Windows. The calling convention needs to be __stdcall:

```
#elif defined(COMPILER_GCC)

#define CEF_EXPORT __attribute__ ((visibility("default")))

#ifdef OS_WIN
#define CEF_CALLBACK __stdcall
#else
#define CEF_CALLBACK
#endif
```
