// Copyright (c) 2014 The cefcapi authors. All rights reserved.
// License: BSD 3-clause.
// Website: https://github.com/CzarekTomczak/cefcapi

#pragma once

#include "include/capi/cef_base_capi.h"

// Set to 1 to check if add_ref() and release()
// are called and to track the total number of calls.
// add_ref will be printed as "+", release as "-".
#define DEBUG_REFERENCE_COUNTING 0

// Print only the first execution of the callback,
// ignore the subsequent.
#define DEBUG_CALLBACK(x) { static int first_call = 1; if (first_call == 1) { first_call = 0; printf(x); } }

// ----------------------------------------------------------------------------
// Initialize CEF_CALLBACK methods on a CEF structure.
// May be a structure like _cef_base_t or a handler 
// with callbacks.
// ----------------------------------------------------------------------------

void initialize_cef_callbacks(void* ptr, const void* methods[]) {
    printf("initialize_cef_callbacks, ");
    int i;
    for (i = 0; methods[i] != NULL; i++) {
        memcpy(ptr, &methods[i], sizeof(void*));
        ptr += sizeof(void*);
    }
    printf("%d callbacks\n", i);
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
    DEBUG_CALLBACK("cef_base_t.add_ref\n");
    if (DEBUG_REFERENCE_COUNTING)
        printf("+");
    return 1;
}

///
// Decrement the reference count.  Delete this object when no references
// remain.
///
int CEF_CALLBACK release(struct _cef_base_t* self) {
    DEBUG_CALLBACK("cef_base_t.release\n");
    if (DEBUG_REFERENCE_COUNTING)
        printf("-");
    return 1;
}

///
// Returns the current number of references.
///
int CEF_CALLBACK get_refct(struct _cef_base_t* self) {
    DEBUG_CALLBACK("cef_base_t.get_refct\n");
    if (DEBUG_REFERENCE_COUNTING)
        printf("=");
    return 1;
}

void initialize_cef_base(void* ptr) {
    printf("initialize_cef_base\n");
    // Check if "size" member was set.
    size_t size = *(size_t*)ptr;
    // Let's debug the size in case sizeof was used
    // on a pointer instead of a structure.
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
