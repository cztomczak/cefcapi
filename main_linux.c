// Copyright (c) 2014 The cefcapi authors. All rights reserved.
// License: BSD 3-clause.
// Website: https://github.com/CzarekTomczak/cefcapi

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cef_base.h"
#include "cef_app.h"
#include "cef_client.h"
#include "gtk.h"

int main(int argc, char** argv) {
    // Main args.
    cef_main_args_t mainArgs = {};
    mainArgs.argc = argc;
    mainArgs.argv = argv;
    
    // Application handler and its callbacks.
    // cef_app_t structure must be filled. It must implement
    // reference counting. You cannot pass a structure 
    // initialized with zeroes.
    cef_app_t app = {};
    initialize_app_handler(&app);
    
    // Execute subprocesses.
    printf("cef_execute_process, argc=%d\n", argc);
    int code = cef_execute_process(&mainArgs, &app, NULL);
    if (code >= 0) {
        _exit(code);
    }
    
    // Application settings.
    // It is mandatory to set the "size" member.
    cef_settings_t settings = {};
    settings.size = sizeof(cef_settings_t);
    settings.no_sandbox = 1;

    // Initialize CEF.
    printf("cef_initialize\n");
    cef_initialize(&mainArgs, &settings, &app, NULL);

    // Create GTK window. You can pass a NULL handle 
    // to CEF and then it will create a window of its own.
    initialize_gtk();
    GtkWidget* hwnd = create_gtk_window("cefcapi example", 1024, 768);
    cef_window_info_t windowInfo = {};
    windowInfo.parent_widget = hwnd;

    // Executable's directory
    char appPath[1024] = {};
    ssize_t ppLen = readlink("/proc/self/exe", appPath,
            sizeof(appPath)-1);
    if (ppLen != -1 && ppLen > 0) {
        appPath[ppLen] = '\0';
        do {
            ppLen -= 1;
            appPath[ppLen+1] = '\0';
        } while (appPath[ppLen] != '/' && ppLen > 0);
        // No slash at the end.
        if (ppLen > 0 && appPath[ppLen] == '/') {
            appPath[ppLen] = '\0';
        }
    }
    printf("Executable's directory: %s\n", appPath);
    
    // Initial url.
    char url[1024];
    snprintf(url, sizeof(url), "file://%s/example.html", appPath);
    // There is no _cef_string_t type.
    cef_string_t cefUrl = {};
    cef_string_utf8_to_utf16(url, strlen(url), &cefUrl);
    
    // Browser settings.
    // It is mandatory to set the "size" member.
    cef_browser_settings_t browserSettings = {};
    browserSettings.size = sizeof(cef_browser_settings_t);
    
    // Client handler and its callbacks.
    // cef_client_t structure must be filled. It must implement
    // reference counting. You cannot pass a structure 
    // initialized with zeroes.
    cef_client_t client = {};
    initialize_client_handler(&client);

    // Create browser.
    printf("cef_browser_host_create_browser\n");
    cef_browser_host_create_browser(&windowInfo, &client, &cefUrl,
            &browserSettings, NULL);

    // Message loop.
    printf("cef_run_message_loop\n");
    cef_run_message_loop();

    // Shutdown CEF.
    printf("cef_shutdown\n");
    cef_shutdown();

    return 0;
}
