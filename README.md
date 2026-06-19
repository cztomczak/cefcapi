# CEF C API (cefcapi)

A simple example of how to use the C API in the [Chromium Embedded
Framework (CEF)](https://github.com/chromiumembedded/cef), originally created by
[Czarek Tomczak](https://www.linkedin.com/in/czarektomczak/).
The example creates a window, embeds a browser and loads the Google website.
It shows how to implement CEF structures with callbacks for handlers such as
`cef_app_t`, `cef_client_t` and `cef_life_span_handler_t`. The example primarily
uses dummy reference counting for simplicity, but `cef_load_handler.h`
and `reference_counting.h` also demonstrate how real reference counting
can be implemented. To better understand real reference counting see the
[Using The CAPI](https://chromiumembedded.github.io/cef/using_the_capi)
wiki page in the upstream CEF project.

Table of contents:
* [Examples](#examples)
* [Compatibility](#compatibility)
* [Getting started](#getting-started)
* [Support development](#support-development)

## Examples

- Windows: see [main_win.c](examples/main_win.c)
- Linux: see [main_linux.c](examples/main_linux.c)
- Mac: not available yet ([Issue #15](../../issues/15))

## Compatibility

Tested configurations:
- Windows:
    - Binary: `cef_binary_148.0.10+g7ee53f5+chromium-148.0.7778.218_windows64.tar.bz2`
    - Compilers: mingw-w64 gcc 12.4.0 and Visual Studio 2022 on Windows 11 64-bit
- Linux:
    - Binary: `cef_binary_145.0.25+g265860d+chromium-145.0.7632.76_linux64.tar.bz2`
    - Compiler: gcc 11.4.0 on Ubuntu 22.04 64-bit

## Getting started

1. Download "Standard Distribution" binaries from Spotify
   Automated Builds for the supported CEF version mentioned
   in the Compatibility section above:
   https://cef-builds.spotifycdn.com/index.html
2. Put the downloaded `cef_binary_*.tar.bz2` file into the `third_party/` directory
3. On Linux run `make` (example uses GTK+ 3)
4. On Windows run `build_msvc.bat` or `build_gcc.bat` to build with MSVC or MinGW, respectively
    - 7zip will need to be installed, or you will need to extract cef manually
5. The built application will appear in the `Release` directory at repo top level

## Support development

If you would like to support general CEF C API development efforts
by making a donation please click the Paypal Donate button:

<a href='https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=YHSHWR5JM4FN4'>
<img src='https://raw.githubusercontent.com/wiki/cztomczak/cefpython/images/donate.gif' />
</a><br><br>

