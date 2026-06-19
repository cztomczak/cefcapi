#pragma once

#include "cef_base.h"
#include "include/capi/cef_load_handler_capi.h"

// A demonstration of how one might implement reference counting.
// Our custom cef_load_handler doesn't need reference counting, though, and
// could just use the same "dummy" reference counting used by the other
// handler implementations

struct my_load_handler {
    // We "extend" `cef_load_handler_t`, which in turn "extends"
    // `cef_base_ref_counted_t`.
    cef_load_handler_t base;

    // This member must exist in any struct which implements
    // reference counting using `reference_counting.h`
    struct reference_counter rc;
};

// See `reference_counting.h` for the meaning of these defines
#define REFERENCE_COUNTING_IMPLEMENTATION
#define T struct my_load_handler
#define ID(n) load_handler_ ## n
#include "reference_counting.h"

void CEF_CALLBACK on_load_error(
    cef_load_handler_t *self_in,
    cef_browser_t *browser,
    cef_frame_t *frame,
    cef_errorcode_t errorCode,
    cef_string_t const *errorText,
    cef_string_t const *failedUrl
){
    DEBUG_CALLBACK;
    printf("Failed to load page: %d\n", errorCode);

    cef_string_t fail_url = cef_string_literal("data:text/plain;charset=utf-8,Failed to Load Page");
    frame->load_url(frame, &fail_url);

    browser->base.release(&browser->base);
    frame->base.release(&frame->base);
}

struct my_load_handler *new_load_handler(void){
    struct my_load_handler *ret = load_handler_create_and_initialize_reference_counting();
    if(ret){
        ret->base.on_load_error = &on_load_error;
    }
    return ret;
}

