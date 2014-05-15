cefcapi - CEF C API
===================
A simple example on how to use the C API in [Chromium Embedded
Framework](https://code.google.com/p/chromiumembedded/).
Example creates a window, embeds a browser and displays local
html file. Shows how to implement dummy reference
counting and CEF structures with callbacks to 
implement CEF handlers like cef_app_t and cef_client_t.

Examples available for both Linux and Windows.

Tested and works fine with CEF 3 branch 1750 revision 1629
(Chrome stable 33.0.1750.117).

TODO: create python script that will generate callbacks for all
handlers, and a corresponding initialize_struct_name() functions
for each handler.


Getting started
---------------
1. Copy binaries to the Release/ directory.
2. Run the "make" command on Linux. On Windows use the build.bat script.


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
