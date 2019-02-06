#ifndef _GUI_LOADER_H
#define _GUI_LOADER_H

#include "log.h"
#include <gtk/gtk.h>
#include <vte/vte.h>

enum
{
    COL_NAME = 0,
    COL_SOCKET,
    COL_IPV4,
    COL_STATUS,
    NUM_COLS
};

void
load_clients_list(GtkBuilder *builder, GtkListStore *store, GtkWidget *client_tree);

void
load_terminal(GtkBuilder *builder);

void
load_logs(GtkBuilder *builder, GtkTextView *text_view);

void
load_interface(GtkBuilder *builder, GtkTextView *text_view, GtkListStore *store, GtkWidget *client_tree);
#endif
