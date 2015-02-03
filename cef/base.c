#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "cef/base.h"
#include "include/capi/cef_base_capi.h"

CEF_CALLBACK void add_ref(struct _cef_base_t* self)
{
	struct refcount *r;
	char *cp;

	cp = (char*)self;
	cp -= sizeof(struct refcount);
	r = (struct refcount*)cp;

	if ((errno = pthread_mutex_lock(&r->lock))) {
		printf("failed to lock mutex");
		return;
	}
	r->refs++;
	if ((errno = pthread_mutex_unlock(&r->lock))) {
		printf("failed to unlock mutex");
	}
}

CEF_CALLBACK int release(struct _cef_base_t* self)
{
	struct refcount *r;
	char *cp;

	cp = (char*)self;
	cp -= sizeof(struct refcount);
	r = (struct refcount*)cp;

	if ((errno = pthread_mutex_lock(&r->lock))) {
		printf("failed to lock mutex");
		return 0;
	}
	if (0 == --(r->refs)) {
		pthread_mutex_unlock(&r->lock);
		pthread_mutex_destroy(&r->lock);
		free(r);
		return 1;
	}
	if ((errno = pthread_mutex_unlock(&r->lock))) {
		printf("failed to unlock mutex");
	}
	return 0;
}

CEF_CALLBACK int has_one_ref(struct _cef_base_t* self)
{
	struct refcount *r;
	char *cp;

	cp = (char*)self;
	cp -= sizeof(struct refcount);
	r = (struct refcount*)cp;

	return r->refs == 1;
}

int init_base(struct _cef_base_t* self, size_t sz)
{
	struct refcount *r;
	char *cp;

	cp = (char*)self;
	cp -= sizeof(struct refcount);
	r = (struct refcount*)cp;

	if ((errno = pthread_mutex_init(&r->lock, NULL))) {
		printf("failed to init mutex");
		return 0;
	}
	self->size = sz;
	self->add_ref = &add_ref;
	self->release = &release;
	self->has_one_ref = &has_one_ref;
	return 1;
}
