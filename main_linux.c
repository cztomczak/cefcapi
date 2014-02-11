// Copyright (c) 2014 The cefcapi authors. All rights reserved.
// License: BSD 3-clause.
// Website: https://github.com/CzarekTomczak/cefcapi

#include "include/capi/cef_app_capi.h"
#include "include/capi/cef_client_capi.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void initialize_app_handler(struct _cef_app_t* app);
void initialize_client_handler(struct _cef_client_t* app);

int main(int argc, char** argv) {
    // Main args.
    struct _cef_main_args_t mainArgs = {};
    mainArgs.argc = argc;
    mainArgs.argv = argv;
    
    // Application handler and its callbacks.
    // _cef_app_t structure must be filled. It must implement
    // reference counting. You cannot pass a structure 
    // initialized with zeroes.
    struct _cef_app_t app = {};
    initialize_app_handler(&app);
    
    // Execute subprocesses.
    printf("cef_execute_process, argc=%d\n", argc);
    int code = cef_execute_process(&mainArgs, &app, NULL);
    if (code >= 0) {
        _exit(code);
    }
    
    // Application settings.
    // It is mandatory to set the "size" member.
    struct _cef_settings_t settings = {};
    settings.size = sizeof(struct _cef_settings_t);
    settings.no_sandbox = 1;

    // gtk_init(&argc, &argv);
    
    // Initialize CEF.
    printf("cef_initialize\n");
    cef_initialize(&mainArgs, &settings, &app, NULL);

    // Main window.
    GtkWidget* hwnd = NULL;
    struct _cef_window_info_t windowInfo = {};
    windowInfo.parent_widget = hwnd;
    
    // Initial url.
    const char url[] = "example.html";
    // There is no _cef_string_t type.
    cef_string_t cefUrl = {};
    cef_string_utf8_to_utf16(url, sizeof(url)-1, &cefUrl);
    
    // Browser settings.
    // It is mandatory to set the "size" member.
    struct _cef_browser_settings_t browserSettings = {};
    browserSettings.size = sizeof(struct _cef_browser_settings_t);
    
    // Client handler and its callbacks.
    // _cef_client_t structure must be filled. It must implement
    // reference counting. You cannot pass a structure 
    // initialized with zeroes.
    struct _cef_client_t client = {};
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

// ----------------------------------------------------------------------------
// Initialize CEF_CALLBACK methods on a CEF structure.
// May be a structure like _cef_base_t or a handler 
// with callbacks.
// ----------------------------------------------------------------------------

void initialize_cef_callbacks(void* ptr, const void* methods[]) {
    int i;
    for (i = 0; methods[i] != NULL; i++) {
        memcpy(ptr, methods[i], sizeof(void*));
        ptr += sizeof(void*);
    }
}

// ----------------------------------------------------------------------------
// cef_base_t
// ----------------------------------------------------------------------------

///
// Structure defining the reference count implementation functions. All
// framework structures must include the cef_base_t structure first.
///

///
// Increment the reference count.
///
int CEF_CALLBACK add_ref(struct _cef_base_t* self) {
    printf("cef_base_t.add_ref\n");
    return 0;
}

///
// Decrement the reference count.  Delete this object when no references
// remain.
///
int CEF_CALLBACK release(struct _cef_base_t* self) {
    printf("cef_base_t.release\n");
    return 0;
}

///
// Returns the current number of references.
///
int CEF_CALLBACK get_refct(struct _cef_base_t* self) {
    printf("cef_base_t.get_refct\n");
    return 0;
}

void initialize_cef_base(void* ptr) {
    printf("initialize_cef_base\n");
    // Check if "size" member was set.
    size_t size = *(size_t*)ptr;
    printf("cef_base_t.size = %lu\n", size);
    static struct _cef_base_t cef_base_tmp;
    if (sizeof(size) != sizeof(cef_base_tmp.size)) {
        printf("FATAL: _cef_base_t.size has invalid type\n");
        _exit(1);
    }
    if (size <= 0) {
        printf("FATAL: initialize_cef_base failed, size member not set\n");
        _exit(1);
    }
    ptr += sizeof(cef_base_tmp.size);
    const void* methods[] = {
        &add_ref,
        &release,
        &get_refct,
        NULL
    };
    initialize_cef_callbacks(ptr, methods);
}

// ----------------------------------------------------------------------------
// struct _cef_app_t
// ----------------------------------------------------------------------------

///
// Implement this structure to provide handler implementations. Methods will be
// called by the process and/or thread indicated.
///

///
// Provides an opportunity to view and/or modify command-line arguments before
// processing by CEF and Chromium. The |process_type| value will be NULL for
// the browser process. Do not keep a reference to the cef_command_line_t
// object passed to this function. The CefSettings.command_line_args_disabled
// value can be used to start with an NULL command-line object. Any values
// specified in CefSettings that equate to command-line arguments will be set
// before this function is called. Be cautious when using this function to
// modify command-line arguments for non-browser processes as this may result
// in undefined behavior including crashes.
///
void CEF_CALLBACK on_before_command_line_processing(
        struct _cef_app_t* self, const cef_string_t* process_type,
        struct _cef_command_line_t* command_line) {
    printf("on_before_command_line_processing\n");
}

///
// Provides an opportunity to register custom schemes. Do not keep a reference
// to the |registrar| object. This function is called on the main thread for
// each process and the registered schemes should be the same across all
// processes.
///
void CEF_CALLBACK on_register_custom_schemes(
        struct _cef_app_t* self,
        struct _cef_scheme_registrar_t* registrar) {
    printf("on_register_custom_schemes\n");
}

///
// Return the handler for resource bundle events. If
// CefSettings.pack_loading_disabled is true (1) a handler must be returned.
// If no handler is returned resources will be loaded from pack files. This
// function is called by the browser and render processes on multiple threads.
///
struct CEF_CALLBACK _cef_resource_bundle_handler_t* 
        get_resource_bundle_handler(struct _cef_app_t* self) {
    printf("get_resource_bundle_handler\n");
    return NULL;
}

///
// Return the handler for functionality specific to the browser process. This
// function is called on multiple threads in the browser process.
///
struct CEF_CALLBACK _cef_browser_process_handler_t* 
        get_browser_process_handler(struct _cef_app_t* self) {
    printf("get_browser_process_handler\n");
    return NULL;
}

///
// Return the handler for functionality specific to the render process. This
// function is called on the render process main thread.
///
struct CEF_CALLBACK _cef_render_process_handler_t* get_render_process_handler(
        struct _cef_app_t* self) {
    printf("get_render_process_handler\n");
    return NULL;
}

void initialize_app_handler(struct _cef_app_t* app) {
    printf("initialize_app_handler\n");
    app->base.size = sizeof(struct _cef_app_t);
    initialize_cef_base(app);
    // callbacks
    const void* methods[] = {
        &on_before_command_line_processing,
        &on_register_custom_schemes,
        &get_resource_bundle_handler,
        &get_browser_process_handler,
        &get_render_process_handler,
        NULL
    };
    void* ptr = app + sizeof(struct _cef_base_t);
    initialize_cef_callbacks(ptr, methods);
}

// ----------------------------------------------------------------------------
// struct _cef_client_t
// ----------------------------------------------------------------------------

///
// Implement this structure to provide handler implementations.
///

///
// Return the handler for context menus. If no handler is provided the default
// implementation will be used.
///

struct CEF_CALLBACK _cef_context_menu_handler_t* get_context_menu_handler(
        struct _cef_client_t* self) {
    printf("get_context_menu_handler\n");
    return NULL;
}

///
// Return the handler for dialogs. If no handler is provided the default
// implementation will be used.
///
struct CEF_CALLBACK _cef_dialog_handler_t* get_dialog_handler(
        struct _cef_client_t* self) {
    printf("get_dialog_handler\n");
    return NULL;
}

///
// Return the handler for browser display state events.
///
struct CEF_CALLBACK _cef_display_handler_t* get_display_handler(
        struct _cef_client_t* self) {
    printf("get_display_handler\n");
    return NULL;
}

///
// Return the handler for download events. If no handler is returned downloads
// will not be allowed.
///
struct CEF_CALLBACK _cef_download_handler_t* get_download_handler(
        struct _cef_client_t* self) {
    printf("get_download_handler\n");
    return NULL;
}

///
// Return the handler for drag events.
///
struct CEF_CALLBACK _cef_drag_handler_t* get_drag_handler(
        struct _cef_client_t* self) {
    printf("get_drag_handler\n");
    return NULL;
}

///
// Return the handler for focus events.
///
struct CEF_CALLBACK _cef_focus_handler_t* get_focus_handler(
        struct _cef_client_t* self) {
    printf("get_focus_handler\n");
    return NULL;
}

///
// Return the handler for geolocation permissions requests. If no handler is
// provided geolocation access will be denied by default.
///
struct CEF_CALLBACK _cef_geolocation_handler_t* get_geolocation_handler(
        struct _cef_client_t* self) {
    printf("get_geolocation_handler\n");
    return NULL;
}

///
// Return the handler for JavaScript dialogs. If no handler is provided the
// default implementation will be used.
///
struct CEF_CALLBACK _cef_jsdialog_handler_t* get_jsdialog_handler(
        struct _cef_client_t* self) {
    printf("get_jsdialog_handler\n");
    return NULL;
}

///
// Return the handler for keyboard events.
///
struct CEF_CALLBACK _cef_keyboard_handler_t* get_keyboard_handler(
        struct _cef_client_t* self) {
    printf("get_keyboard_handler\n");
    return NULL;
}

///
// Return the handler for browser life span events.
///
struct CEF_CALLBACK _cef_life_span_handler_t* get_life_span_handler(
        struct _cef_client_t* self) {
    printf("get_life_span_handler\n");
    return NULL;
}

///
// Return the handler for browser load status events.
///
struct CEF_CALLBACK _cef_load_handler_t* get_load_handler(
        struct _cef_client_t* self) {
    printf("get_load_handler\n");
    return NULL;
}

///
// Return the handler for off-screen rendering events.
///
struct CEF_CALLBACK _cef_render_handler_t* get_render_handler(
        struct _cef_client_t* self) {
    printf("get_render_handler\n");
    return NULL;
}

///
// Return the handler for browser request events.
///
struct CEF_CALLBACK _cef_request_handler_t* get_request_handler(
        struct _cef_client_t* self) {
    printf("get_request_handler\n");
    return NULL;
}

///
// Called when a new message is received from a different process. Return true
// (1) if the message was handled or false (0) otherwise. Do not keep a
// reference to or attempt to access the message outside of this callback.
///
int CEF_CALLBACK on_process_message_received(
        struct _cef_client_t* self,
        struct _cef_browser_t* browser, cef_process_id_t source_process,
        struct _cef_process_message_t* message) {
    printf("on_process_message_received\n");
    return 0;
}

void initialize_client_handler(struct _cef_client_t* client) {
    printf("initialize_client_handler\n");
    client->base.size = sizeof(struct _cef_client_t);
    initialize_cef_base(client);
    // callbacks
    const void* methods[] = {
        &get_context_menu_handler,
        &get_dialog_handler,
        &get_display_handler,
        &get_download_handler,
        &get_drag_handler,
        &get_focus_handler,
        &get_geolocation_handler,
        &get_jsdialog_handler,
        &get_keyboard_handler,
        &get_life_span_handler,
        &get_load_handler,
        &get_render_handler,
        &get_request_handler,
        &on_process_message_received,
        NULL
    };
    void* ptr = client + sizeof(struct _cef_client_t);
    initialize_cef_callbacks(ptr, methods);
}
