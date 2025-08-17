// CEF C API example
// Project website: https://github.com/cztomczak/cefcapi

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <gdk/gdkx.h>

#include "gtk.h"

static size_t wide_string_len(char16_t *s){
    size_t ret = 0;
    if(s){
        while(s[ret]) ret++;
    }
    return ret;
}

static cef_string_t cef_string_wide_literal(char16_t *s){
    cef_string_t ret = {0};
    cef_string_set(s, wide_string_len(s), &ret, 0);
    return ret;
}

#define cef_string_literal(s) cef_string_wide_literal(u ## s)

#define platform_atomic _Atomic
#define platform_atomic_increment(pint) (++*(pint))
#define platform_atomic_decrement(pint) (--*(pint))
#define platform_atomic_load(pint) (*(pint))

#include "capi/cef_base.h"
#include "capi/cef_app.h"
#include "capi/cef_client.h"
#include "capi/cef_life_span_handler.h"

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
    cef_main_args_t main_args = {
        .argc = argc,
        .argv = argv
    };

    // Handshake with the CEF library so we know there's no version mismatch
    char const *api_hash = cef_api_hash(CEF_API_VERSION, 0);
    if(0 != strcmp(api_hash, CEF_API_HASH_PLATFORM)){
        fprintf(stderr,
            "The version of the provided libcef.so doesn't match the version this "
            "application was built against\n"
            "Our API hash: %s\n"
            "Library's API hash: %s\n",
            CEF_API_HASH_PLATFORM,
            api_hash);
        _exit(1);
    }

    struct my_cef_app app = {0};
    initialize_cef_app(&app);

    // Execute subprocesses. It is also possible to have
    // a separate executable for subprocesses by setting
    // cef_settings_t.browser_subprocess_path. In such
    // case cef_execute_process should not be called here.
    printf("cef_execute_process, argc=%d\n", argc);
    int code = cef_execute_process(&main_args, &app.base, NULL);
    if (code >= 0) {
        _exit(code);
    }

    // Application settings. It is mandatory to set the
    // "size" member.
    cef_settings_t settings = {
        .size = sizeof settings,
        .log_severity = LOGSEVERITY_WARNING, // Show only warnings/errors
        .no_sandbox = 1
        //TODO: .root_cache_path = ...
    };

    // Initialize CEF.
    printf("cef_initialize\n");
    if(cef_initialize(&main_args, &settings, &app.base, NULL)){
        initialize_gtk();
        cef_window_info_t window_info = {
            .size = sizeof window_info,
            .window_name = cef_string_literal("CEF C Example"),
            .runtime_style = CEF_RUNTIME_STYLE_ALLOY // Disable Chrome's chrome.
        };

        // Copied from upstream cefclient. Install xlib error
        // handlers so that the application won't be terminated on
        // non-fatal errors. Must be done after initializing GTK.
        XSetErrorHandler(x11_error_handler);
        XSetIOErrorHandler(x11_io_error_handler);

        // Initial url
        cef_string_t cef_url = cef_string_literal("https://www.google.com/ncr");

        // Browser settings. It is mandatory to set the
        // "size" member.
        cef_browser_settings_t browser_settings = {
            .size = sizeof browser_settings
        };

        // Client handler and its callbacks
        struct my_client client = {0};
        initialize_cef_client(&client);

        // Create browser asynchronously. There is also a
        // synchronous version of this function available.
        printf("cef_browser_host_create_browser\n");
        if(cef_browser_host_create_browser(
            &window_info, &client.base, &cef_url,
            &browser_settings, NULL, NULL
        )){
            // Message loop. There is also cef_do_message_loop_work()
            // that allow for integrating with existing message loops.
            printf("cef_run_message_loop\n");
            cef_run_message_loop();
        }
        // Shutdown CEF
        printf("cef_shutdown\n");
        cef_shutdown();
    }

    printf("Didn't crash!\n");
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
