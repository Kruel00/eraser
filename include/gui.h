//
// Created By: Javier Castorena
//
#include <stdio.h>
#include <stdlib.h>

void alert(gchar *msg);
void second_dialog();
void show_debug_dialog();
int erase_confirm();
void show_help();

typedef struct eraser_config_s {
    char log_server[256];
    char wifi_ssid[512];
    char wifi_pass[512];
} eraser_config_t;