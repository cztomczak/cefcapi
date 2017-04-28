// Copyright (c) 2014 The cefcapi authors. All rights reserved.
// License: BSD 3-clause.
// Project website: https://github.com/cztomczak/cefcapi

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "capi/cef_base.h"
#include "capi/cef_app.h"
#include "capi/cef_client.h"
#include "capi/cef_life_span_handler.h"

#include "include/cef_version_win.h"

// Globals
cef_life_span_handler_t g_life_span_handler = {};


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

    // CEF version
    if (argc == 0) {
        printf("CEF version: %s\n", CEF_VERSION);
    }

    // Main args
    cef_main_args_t main_args = {};
    main_args.instance = GetModuleHandle(NULL);

    // Cef app
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

    // Initialize CEF
    printf("cef_initialize\n");
    cef_initialize(&main_args, &settings, &app, NULL);

    // Window info
    cef_window_info_t window_info = {};
    window_info.style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN \
            | WS_CLIPSIBLINGS | WS_VISIBLE;
    window_info.parent_window = NULL;
    window_info.x = CW_USEDEFAULT;
    window_info.y = CW_USEDEFAULT;
    window_info.width = CW_USEDEFAULT;
    window_info.height = CW_USEDEFAULT;

    // Window info - window title
    char window_name[] = "cefcapi example";
    cef_string_t cef_window_name = {};
    cef_string_utf8_to_utf16(window_name, strlen(window_name),
                             &cef_window_name);
    window_info.window_name = cef_window_name;

    // Initial url
    char url[] = "https://www.google.com/ncr";
    cef_string_t cef_url = {};
    cef_string_utf8_to_utf16(url, strlen(url), &cef_url);

    // Browser settings. It is mandatory to set the
    // "size" member.
    cef_browser_settings_t browser_settings = {};
    browser_settings.size = sizeof(cef_browser_settings_t);
    
    // Client handlers
    cef_client_t client = {};
    initialize_cef_client(&client);
    initialize_cef_life_span_handler(&g_life_span_handler);

    // Create browser asynchronously. There is also a
    // synchronous version of this function available.
    printf("cef_browser_host_create_browser\n");
    cef_browser_host_create_browser(&window_info, &client, &cef_url,
                                    &browser_settings, NULL);

    // Message loop. There is also cef_do_message_loop_work()
    // that allow for integrating with existing message loops.
    // On Windows for best performance you should set
    // cef_settings_t.multi_threaded_message_loop to true.
    // Note however that when you do that CEF UI thread is no
    // more application main thread and using CEF API is more
    // difficult and require using functions like cef_post_task
    // for running tasks on CEF UI thread.
    printf("cef_run_message_loop\n");
    cef_run_message_loop();

    // Shutdown CEF
    printf("cef_shutdown\n");
    cef_shutdown();

    return 0;
}
