#include <stdio.h>
#include <stdlib.h>

#include "common.h"


void alert(gchar *msg)
{
    GtkWidget* hello = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Quantum");
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(hello),"%s", msg);
    int response = gtk_dialog_run(GTK_DIALOG(hello));
    gtk_widget_destroy(hello);

}

void show_debug_dialog(){
    GtkWidget* debug_dialog = gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"DEBUG MODE IS ENABLED!");
    gtk_dialog_run(GTK_DIALOG(debug_dialog));

    gtk_widget_destroy(debug_dialog);
}

int erase_confirm(){
    GtkWidget* confirm_erase = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_YES_NO, "Quantum");
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(confirm_erase),"¿Estás seguro de que quieres eliminar archivos de forma permanente?");
    int response = gtk_dialog_run(GTK_DIALOG(confirm_erase));
    return response;
}

void show_help(){
    GtkWidget* help_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Quantum");
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(help_dialog),"Quantum Help\n\t App_name   /dev/<device>\n");
    int response = gtk_dialog_run(GTK_DIALOG(help_dialog));
}