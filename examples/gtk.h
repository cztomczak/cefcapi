#pragma once

#include "include/capi/cef_app_capi.h"

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

void app_terminate_signal(int signatl) {
    printf("app_terminate_signal\n");
    cef_quit_message_loop();
}

void initialize_gtk() {
    printf("initialize_gtk\n");
    gtk_init(0, NULL);
    signal(SIGINT, app_terminate_signal);
    signal(SIGTERM, app_terminate_signal);
}

void window_destroy_signal(GtkWidget* widget, gpointer data) { 
    printf("window_destroy_signal\n");
    cef_quit_message_loop();
}

GtkWidget* create_gtk_window(char* title, int width, int height) {
    printf("create_gtk_window\n");
    
    // Create window.
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    // Destroy signal.
    g_signal_connect(G_OBJECT(window), "destroy",
            G_CALLBACK(window_destroy_signal), NULL);
    
    // Default size.
    gtk_window_set_default_size(GTK_WINDOW(window),
            width, height);
    
    // Center.
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    
    // Title.
    gtk_window_set_title(GTK_WINDOW(window), title);
    
    // TODO: focus
    // g_signal_connect(window, "focus", G_CALLBACK(&HandleFocus), NULL);

    // CEF requires a container. Embedding browser in a top
    // level window fails.
    GtkWidget* vbox = gtk_vbox_new(0, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    // Show.
    gtk_widget_show_all(window);

    return vbox;
}
