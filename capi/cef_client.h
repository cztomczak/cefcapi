// CEF C API example
// Project website: https://github.com/cztomczak/cefcapi

#pragma once

#include "cef_base.h"
#include "cef_life_span_handler.h"
#include "cef_load_handler.h"
#include "include/capi/cef_client_capi.h"

// ----------------------------------------------------------------------------
// struct cef_client_t
// ----------------------------------------------------------------------------

///
// Implement this structure to provide handler implementations.
///

struct my_client {
    cef_client_t base;
    cef_life_span_handler_t life_span_handler;
};


///
// Return the handler for browser life span events.
///
struct _cef_life_span_handler_t* CEF_CALLBACK get_life_span_handler(
        struct _cef_client_t* self) {
    DEBUG_CALLBACK;
    // add_ref not needed here because our life_span_handler isn't reference counted
    return &container_of(self, struct my_client, base)->life_span_handler;
}

struct _cef_load_handler_t *CEF_CALLBACK get_load_handler(struct _cef_client_t *self){
    DEBUG_CALLBACK;
    return &new_load_handler()->base;
}

void initialize_cef_client(struct my_client *client) {
    DEBUG_CALLBACK;
    initialize_fake_reference_counting(&client->base);

    initialize_cef_life_span_handler(&client->life_span_handler);

    // callbacks
    client->base.get_life_span_handler = get_life_span_handler;  // Implemented!
    client->base.get_load_handler = get_load_handler;
}
