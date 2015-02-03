#ifndef _CEFCAPI_BASE_H
#define _CEFCAPI_BASE_H

#include <pthread.h>
#include "include/capi/cef_base_capi.h"

// increment/decrement refcounters
#define RINC(p) ((cef_base_t*)p)->add_ref((cef_base_t*)p)
#define RDEC(p) ((cef_base_t*)p)->release((cef_base_t*)p)

#define DEBUG_ONCE(str) do{ static int first_call = 1; if (first_call) { first_call = 0; printf("debug_once: %s", str); } }while(0)

struct refcount {
    size_t refs;
    pthread_mutex_t lock;
};

CEF_CALLBACK void add_ref(struct _cef_base_t* self);
CEF_CALLBACK int release(struct _cef_base_t* self);
CEF_CALLBACK int has_one_ref(struct _cef_base_t* self);
int init_base(struct _cef_base_t* self, size_t sz);

#endif
