cefcapi - CEF C API
===================
A simple example on how to use the C API in [Chromium Embedded
Framework](https://code.google.com/p/chromiumembedded/).
Example creates a window, embeds a browser and displays local
the example.html file. Shows how to implement dummy reference
counting, how to initialize CEF structures with callbacks to 
implement CEF handlers like cef_app_t and cef_client_t.

There is only available example for Linux as of now.

Tested and works fine with CEF 3 branch 1750 revision 1604.

TODO: create python script that will generate callbacks for all
handlers, and a corresponding initialize_struct_name() functions
for each handler.


Getting started
---------------
1. Copy binaries to the Release/ directory.
2. Run "make" command.
