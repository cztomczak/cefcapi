
// A generic reference counting implementation for use
// by structs extending `cef_base_ref_counted_t`.  For an
// example of how to use this, see `cef_load_handler`.

// Just including this header only provides the global
// declarations.  To specialize it for a handler
// implementation, you must define the `T` and `ID` macros.

// MANDATORY MACROS:
//  - T: The type of the struct to implement reference counting for
//  - ID(n): All identifiers declared by this header will be passed
//    to ID so they can be given a prefix
//  - REFERENCE_COUNTING_IMPLEMENTATION: define this
//    to emit function definitions.  Do not define it in more than
//    one compilation unit.

// OPTIONAL MACROS:
//  - BASE_NAME: the name of the member of `T` whose size we should assign
//    to the BASE_RC_NAME member's .size field (defaults to `base`)
//  - BASE_RC_NAME: the name of the member of `T` with type
//    `cef_base_ref_counted_t` (defaults to `base.base`)
//  - RC_NAME: the name of the member of `T` with type
//    `struct reference_counter` (defaults to `rc`)
//  - ON_DESTROY(self): will be called just before the memory for the struct
//    is freed

#ifndef REFERENCE_COUNTING_H
#define REFERENCE_COUNTING_H
    struct reference_counter {
        platform_atomic long count;
    };
#endif

#if defined(T) && defined(ID)

    // Allocate an instance of this struct using malloc,
    // zero out all fields, then initialize reference
    // counting.  The returned struct will start with a
    // reference count of 1.  Returns NULL if memory
    // for the struct could not be allocated.
    T *ID(create_and_initialize_reference_counting)(void);

    #ifdef REFERENCE_COUNTING_IMPLEMENTATION
        #ifndef BASE_NAME
        #define BASE_NAME base
        #endif

        #ifndef BASE_RC_NAME
        #define BASE_RC_NAME BASE_NAME.base
        #endif

        #ifndef RC_NAME
        #define RC_NAME rc
        #endif

        #ifndef ON_DESTROY
        #define ON_DESTROY(...) ((void)0)
        #endif

        static void CEF_CALLBACK ID(add_ref)(cef_base_ref_counted_t *self_in){
            if(DEBUG_REFERENCE_COUNTING) printf("%p +\n", (void *)self_in);
            struct reference_counter *rc = &container_of(self_in, T, BASE_RC_NAME)->RC_NAME;
            platform_atomic_increment(&rc->count);
        }

        static int CEF_CALLBACK ID(release)(cef_base_ref_counted_t *self_in){
            if(DEBUG_REFERENCE_COUNTING) printf("%p -\n", (void *)self_in);
            T *self = container_of(self_in, T, BASE_RC_NAME);
            long new_value = platform_atomic_decrement(&self->RC_NAME.count);
            if(new_value == 0){
                ON_DESTROY(self);
                free(self);
            }
            return new_value == 0;
        }

        static int CEF_CALLBACK ID(has_one_ref)(cef_base_ref_counted_t *self_in){
            struct reference_counter *rc = &container_of(self_in, T, BASE_RC_NAME)->RC_NAME;
            return platform_atomic_load(&rc->count) == 1;
        }

        static int CEF_CALLBACK ID(has_at_least_one_ref)(cef_base_ref_counted_t *self_in){
            struct reference_counter *rc = &container_of(self_in, T, BASE_RC_NAME)->RC_NAME;
            return platform_atomic_load(&rc->count) >= 1;
        }

        T *ID(create_and_initialize_reference_counting)(void){
            T *self = malloc(sizeof *self);
            if(self){
                *self = (T){
                    .BASE_RC_NAME = {
                        .size = sizeof self->BASE_NAME,
                        .add_ref = &ID(add_ref),
                        .release = &ID(release),
                        .has_one_ref = &ID(has_one_ref),
                        .has_at_least_one_ref = &ID(has_at_least_one_ref)
                    },
                    .RC_NAME = {
                        .count = 1
                    }
                };
            }
            return self;
        }
        #undef BASE_RC_NAME
        #undef BASE_NAME
        #undef RC_NAME
        #undef ON_DESTROY
    #endif
    #undef T
    #undef ID
#endif
