// CEF C API example
// Project website: https://github.com/cztomczak/cefcapi

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
    #if GTK_CHECK_VERSION(3,0,0)
    printf("initialize_gtk %u.%u.%u\n", gtk_get_major_version(), gtk_get_minor_version(), gtk_get_micro_version());
    #else
    printf("initialize_gtk 2\n");
    #endif
    gtk_init(0, NULL);
    signal(SIGINT, app_terminate_signal);
    signal(SIGTERM, app_terminate_signal);
}

void window_destroy_signal(GtkWidget* widget, gpointer data) { 
    printf("window_destroy_signal\n");
    cef_quit_message_loop();
}


static void fix_default_x11_visual(GtkWidget* widget) {
    #if GTK_CHECK_VERSION(3,15,1)
    // GTK+ > 3.15.1 uses an X11 visual optimized for GTK+'s OpenGL stuff
    // since revid dae447728d: https://github.com/GNOME/gtk/commit/dae447728d
    // However, it breaks CEF: https://github.com/cztomczak/cefcapi/issues/9
    // Let's use the default X11 visual instead of the GTK's blessed one.
    GdkScreen* screen = gdk_screen_get_default();
    GList* visuals = gdk_screen_list_visuals(screen);
    GdkX11Screen* x11_screen = GDK_X11_SCREEN(screen);
    g_return_if_fail(x11_screen != NULL);
    Visual* default_xvisual = DefaultVisual(GDK_SCREEN_XDISPLAY(x11_screen), GDK_SCREEN_XNUMBER(x11_screen));
    GList* cursor = visuals;
    while (cursor != NULL) {
        GdkVisual* visual = GDK_X11_VISUAL(cursor->data);
        if (default_xvisual->visualid == gdk_x11_visual_get_xvisual(visual)->visualid) {
            gtk_widget_set_visual(widget, visual);
            break; 
        }
        cursor = cursor->next;
    }
    g_list_free(visuals);
    #endif
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
    #if GTK_CHECK_VERSION(3,0,0)
    GtkWidget* vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    #else
    GtkWidget* vbox = gtk_vbox_new(0, 0);
    #endif
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    // Show.
    fix_default_x11_visual(GTK_WIDGET(window));
    gtk_widget_show_all(window);

    return vbox;
}
