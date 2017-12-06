# CEF C API (cefcapi)

A simple example on how to use the C API in [Chromium Embedded
Framework](https://bitbucket.org/chromiumembedded/cef)
(in short CEF). Example creates a window, embeds a browser
inside that window and loads Google website. Shows how to
implement dummy reference counting and CEF structures with
callbacks to implement CEF handlers like cef_app_t,
cef_client_t and cef_life_span_handler_t.

Table of contents:
* [Examples](#examples)
* [Compatibility](#compatibility)
* [Getting started](#getting-started)
* [Updating CEF version](#updating-cef-version)
* [Patch for gcc compiler on Windows](#patch-for-gcc-compiler-on-windows)
* [Support development](#support-development)


## Examples

- Windows: see [main_win.c](examples/main_win.c)
- Linux: see [main_linux.c](examples/main_linux.c)
- Mac: not available yet


## Compatibility

Tested configurations:
- Windows:
    - Binary: `cef_binary_3.3202.1692.g18a939d_windows32.tar.bz2`
    - Compilers: mingw-gcc 5.3.0 and TDM-gcc 5.1.0 on Windows 7 64-bit
- Linux:
    - Binary: `cef_binary_3.3202.1692.g18a939d_linux64.tar.bz2`
    - Compiler: Linux: gcc 4.8.2 on Ubuntu 14.04 64-bit
    - Compiler: gcc 6.3.0 on Debian 9.2 (stretch) 64-bit


## Getting started

1. Download "Standard Distribution" binaries from Spotify
   Automated Builds for the supported CEF version mentioned
   in the Compatibility section above:
   http://opensource.spotify.com/cefbuilds/index.html
2. Create cefcapi/Release/ directory
3. Copy cef_binary*/Release/* to cefcapi/Release/
4. Copy cef_binary*/Resources/* to cefcapi/Release/
5. On Linux run "make gtk2" (GTK+ 2 example) or "make gtk3" (GTK+ 3 example) in the root directory.
   Note that you may need to build CEF with "use_gtk3=true" to link it with GTK+ 3.
6. On Windows run the "build.bat" script in the root directory


## Updating CEF version

If you download a CEF version newer than `3.3202.1692` then
apart from copying binaries to cefcapi/Release/ directory
you will also have to update the include/ directory with CEF
header files.

In the include/ directory you can find two files: "cef_version_win.h"
and "cef_version_linux.h" - these header files were copied
from CEF binary distributions for appropriate platforms and
their original file names were "cef_version.h".

## Support development

If you would like to support general cefcapi development efforts
by making a donation please click the Paypal Donate button:

<a href='https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=V7LU7PD4N4GGG'>
<img src='https://raw.githubusercontent.com/wiki/cztomczak/cefpython/images/donate.gif' />
</a><br><br>
