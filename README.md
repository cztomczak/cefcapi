cefcapi - CEF C API
===================

A simple example on how to use the C API in [Chromium Embedded
Framework](https://bitbucket.org/chromiumembedded/cef)
(in short CEF). Example creates a window, embeds a browser
inside that window and loads Google website. Shows how to
implement dummy reference counting and CEF structures with
callbacks to implement CEF handlers like cef_app_t,
cef_client_t and cef_life_span_handler_t.


Compatibility
-------------

Examples are available for both Linux and Windows.

Tested with CEF branch 3029:
- Linux: `cef_binary_3.3029.1608.g27a32db_linux64.tar.bz2`
- Windows: `cef_binary_3.3029.1608.g27a32db_windows32.tar.bz2`

If you download a CEF version newer than `3.3029.1608` then
apart from copying binaries to cefcapi/Release/ directory
you will also have to update the include/ directory with CEF
header files.

In the include/ directory you can find two files: "cef_version_win.h"
and "cef_version_linux.h" - these header files were copied
from CEF binary distributions and their original file names
were "cef_version.h".

Tested only with gcc compiler:
- Linux: gcc 4.8.2 on Ubuntu 14.04 64-bit
- Windows: mingw-gcc 5.3.0 and TDM-gcc 5.1.0 on Windows 7 64-bit


Getting started
---------------
1. Download "Standard Distribution" binaries from Spotify
   Automated Builds for the supported CEF version mentioned
   in the Compatibility section above:
   http://opensource.spotify.com/cefbuilds/index.html
2. Copy cef_binary*/Release/* to cefcapi/Release/
3. Copy cef_binary*/Resources/* to cefcapi/Release/
4. On Linux run the "make" command
5. On Windows if using GCC compiler apply the patch - see
   the next section "GCC compiler on Windows"
6. On Windows run the "build.bat" script


GCC compiler on Windows
-----------------------
It was required to modify [cef_export.h](include/internal/cef_export.h)
to make it work with GCC compiler on Windows. The calling
convention needs to be __stdcall.

Related issues:
- Issue [#6](../../issues/6)
- Upstream [Issue 1209](https://bitbucket.org/chromiumembedded/cef/issues/1209)

Here is the patch:

```text
diff --git a/include/internal/cef_export.h b/include/internal/cef_export.h
index 2813253b..e78be029 100644
--- a/include/internal/cef_export.h
+++ b/include/internal/cef_export.h
@@ -48,7 +48,11 @@
 #elif defined(COMPILER_GCC)
 
 #define CEF_EXPORT __attribute__ ((visibility("default")))
+#ifdef OS_WIN
+#define CEF_CALLBACK __stdcall
+#else
 #define CEF_CALLBACK
+#endif
 
 #endif  // COMPILER_GCC
```
