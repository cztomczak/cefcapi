// CEF C API example
// Project website: https://github.com/cztomczak/cefcapi

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <gdk/gdkx.h>

#include "gtk.h"

#include "capi/cef_base.h"
#include "capi/cef_app.h"
#include "capi/cef_client.h"
#include "capi/cef_life_span_handler.h"

// Globals
cef_life_span_handler_t g_life_span_handler = {};  // not used currently

// Signatures
int x11_error_handler(Display *display, XErrorEvent *event);
int x11_io_error_handler(Display *display);


int main(int argc, char** argv) {
    // This executable is called many times, because it
    // is also used for subprocesses. Let's print args
    // so we can differentiate between main process and
    // subprocesses. If one of the first args is for
    // example "--type=renderer" then it means that
    // this is a Renderer process. There may be more
    // subprocesses like GPU (--type=gpu-process) and
    // others. On Linux there are also special Zygote
    // processes.
    printf("\nProcess args: ");
    if (argc == 1) {
        printf("none (Main process)");
    } else {
        for (int i = 1; i < argc; i++) {
            if (strlen(argv[i]) > 128)
                printf("... ");
            else
                printf("%s ", argv[i]);
        }
    }
    printf("\n\n");

    // Main args.
    cef_main_args_t main_args = {};
    main_args.argc = argc;
    main_args.argv = argv;
    
    cef_app_t app = {};
    initialize_cef_app(&app);
    
    // Execute subprocesses. It is also possible to have
    // a separate executable for subprocesses by setting
    // cef_settings_t.browser_subprocess_path. In such
    // case cef_execute_process should not be called here.
    printf("cef_execute_process, argc=%d\n", argc);
    int code = cef_execute_process(&main_args, &app, NULL);
    if (code >= 0) {
        _exit(code);
    }
    
    // Application settings. It is mandatory to set the
    // "size" member.
    cef_settings_t settings = {};
    settings.size = sizeof(cef_settings_t);
    settings.log_severity = LOGSEVERITY_WARNING; // Show only warnings/errors
    settings.no_sandbox = 1;

    // Initialize CEF.
    printf("cef_initialize\n");
    cef_initialize(&main_args, &settings, &app, NULL);

    // Create GTK window. Alternatively you can pass a NULL handle
    // to CEF and then it will create a window of its own.
    // When passing NULL you have to implement cef_life_span_handler_t
    // and call cef_quit_message_loop from the on_before_close
    // callback. Example initialization of this handler and its
    // callback is Windows example.
    initialize_gtk();
    GtkWidget* gtk_window = create_gtk_window("cefcapi example", 800, 600);
    cef_window_info_t window_info = {};
    #if GTK_CHECK_VERSION(3,0,0)
    Window xid = gdk_x11_window_get_xid(gtk_widget_get_window(gtk_window));
    #else
    Window xid = gdk_x11_drawable_get_xid(gtk_widget_get_window(gtk_window));
    #endif
    printf("Window xid %u\n", (unsigned) xid);
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

    // Create browser asynchronously. There is also a
    // synchronous version of this function available.
    printf("cef_browser_host_create_browser\n");
    cef_browser_host_create_browser(&window_info, &client, &cef_url,
                                    &browser_settings, NULL);

    // Message loop. There is also cef_do_message_loop_work()
    // that allow for integrating with existing message loops.
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
