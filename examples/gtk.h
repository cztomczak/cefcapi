// CEF C API example
// Project website: https://github.com/cztomczak/cefcapi

#pragma once

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

#include <include/capi/cef_app_capi.h>

void initialize_gtk(void) {
    printf("initialize_gtk %u.%u.%u\n",
        gtk_get_major_version(), gtk_get_minor_version(), gtk_get_micro_version());
    gtk_init(0, NULL);
}
