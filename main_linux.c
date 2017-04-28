// Copyright (c) 2014 The cefcapi authors. All rights reserved.
// License: BSD 3-clause.
// Project website: https://github.com/cztomczak/cefcapi

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <gdk/gdkx.h>

#include "gtk.h"
#include "cef_base.h"
#include "cef_app.h"
#include "cef_client.h"

int x11_error_handler(Display *display, XErrorEvent *event);
int x11_io_error_handler(Display *display);


int main(int argc, char** argv) {
    // Main args.
    cef_main_args_t main_args = {};
    main_args.argc = argc;
    main_args.argv = argv;
    
    cef_app_t app = {};
    initialize_cef_app(&app);
    
    // Execute subprocesses.
    printf("cef_execute_process, argc=%d\n", argc);
    int code = cef_execute_process(&main_args, &app, NULL);
    if (code >= 0) {
        _exit(code);
    }
    
    // Application settings. It is mandatory to set the
    // "size" member.
    cef_settings_t settings = {};
    settings.size = sizeof(cef_settings_t);
    settings.log_severity = LOGSEVERITY_WARNING; // Show only warnings & errors
    settings.no_sandbox = 1;

    // Initialize CEF.
    printf("cef_initialize\n");
    cef_initialize(&main_args, &settings, &app, NULL);

    // Create GTK window. Alternatively you can pass a NULL handle
    // to CEF and then it will create a window of its own.
    // When passing NULL you have to implement cef_life_span_handler_t
    // and call cef_quit_message_loop() from the on_before_close()
    // callback.
    initialize_gtk();
    GtkWidget* gtk_window = create_gtk_window("cefcapi example", 800, 600);
    cef_window_info_t window_info = {};
    Window xid = gdk_x11_drawable_get_xid(gtk_widget_get_window(gtk_window));
    window_info.parent_window = xid;

    // Copied from upstream cefclient. Install xlib error
    // handlers so that the application won't be terminated on
    // non-fatal errors. Must be done after initializing GTK.
    XSetErrorHandler(x11_error_handler);
    XSetIOErrorHandler(x11_io_error_handler);

    // Initial url
    char url[] = "https://www.google.com/ncr";
    cef_string_t cef_url = {};
    cef_string_utf8_to_utf16(url, strlen(url), &cef_url);
    
    // Browser settings. It is mandatory to set the
    // "size" member.
    cef_browser_settings_t browser_settings = {};
    browser_settings.size = sizeof(cef_browser_settings_t);
    
    // Client handler and its callbacks
    cef_client_t client = {};
    initialize_cef_client(&client);

    // Create browser
    printf("cef_browser_host_create_browser\n");
    cef_browser_host_create_browser(&window_info, &client, &cef_url,
                                    &browser_settings, NULL);

    // Message loop
    printf("cef_run_message_loop\n");
    cef_run_message_loop();

    // Shutdown CEF
    printf("cef_shutdown\n");
    cef_shutdown();

    return 0;
}

int x11_error_handler(Display *display, XErrorEvent *event) {
    printf("X11 error: type=%d, serial=%lu, code=%d\n",
           event->type, event->serial, (int)event->error_code);
    return 0;
}

int x11_io_error_handler(Display *display) {
    return 0;
}
