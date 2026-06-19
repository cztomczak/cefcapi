// CEF C API example
// Project website: https://github.com/cztomczak/cefcapi

#pragma once

#include "include/capi/cef_base_capi.h"
#include "reference_counting.h"

// Set to 1 to check if add_ref() and release()
// are called and to track the total number of calls.
// add_ref will be printed as "+", release as "-".
#define DEBUG_REFERENCE_COUNTING 0

// Print only the first execution of the callback,
// ignore the subsequent.
#define DEBUG_CALLBACK do { \
    static int first_call = 1; \
    if (first_call == 1) { \
        first_call = 0; \
        printf("%s\n", __func__); \
    } \
} while(0)

// For a struct within a struct, get a pointer to the "containing"
// struct given a pointer to the "contained" struct.
#define container_of(ptr, type, member) \
    ((type *)((char *)(1?ptr:&((type *)0)->member) - offsetof(type, member)))

// ----------------------------------------------------------------------------
// cef_base_ref_counted_t
// ----------------------------------------------------------------------------

// A "fake" implementation of reference counting for structures which have
// simpler lifetimes.  Most CEF client handlers can use this since they can
// simply outlive the call to cef_shutdown, after which they are guaranteed to no
// longer be referenced by CEF.  For user-defined structs which truly need reference
// counting, see `reference_counting.h`.

void CEF_CALLBACK fake_add_ref(cef_base_ref_counted_t* self) { }
int CEF_CALLBACK fake_release(cef_base_ref_counted_t* self) { return 1; }
int CEF_CALLBACK fake_has_one_ref(cef_base_ref_counted_t* self) { return 1; }
int CEF_CALLBACK fake_has_at_least_one_ref(cef_base_ref_counted_t *self) { return 1; }

static void initialize_fake_reference_counting_impl(cef_base_ref_counted_t* base, size_t size) {
    printf("initialize_cef_base_ref_counted\n");
    base->size = size;
    // Let's print the size in case sizeof was used
    // on a pointer instead of a structure. In such
    // case the number will be very high.
    printf("cef_base_ref_counted_t.size = %lu\n", (unsigned long)size);
    if (size <= 0) {
        printf("FATAL: initialize_cef_base failed, size member not set\n");
        _exit(1);
    }
    base->add_ref = fake_add_ref;
    base->release = fake_release;
    base->has_one_ref = fake_has_one_ref;
    base->has_at_least_one_ref = fake_has_at_least_one_ref;
}

#define initialize_fake_reference_counting(x) \
    initialize_fake_reference_counting_impl(&(x)->base, sizeof *(x))
