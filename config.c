//
// Created By: Javier Castorena
//
#include <stdio.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include "common.h"

#define CONFIG_FILE "config.ini"

const char *readLine(FILE *file);

void load_config(eraser_config_t *const config){
    printf("Loading config..\n");
    gchar config_text[1024];
    char cwd[PATH_MAX];
    getcwd(cwd,sizeof(cwd));

    g_snprintf(config_text, G_N_ELEMENTS(config_text), "%s/%s", cwd, CONFIG_FILE);

    char *text;

    GKeyFile *storage_server_config = g_key_file_new();

    if(g_key_file_load_from_file(storage_server_config, config_text, G_KEY_FILE_NONE, NULL) == TRUE) {
        
        gchar *wifi_ssid = g_key_file_get_string(storage_server_config, "NETWORK", "ssid", NULL);
		gchar *wifi_pass = g_key_file_get_string(storage_server_config, "NETWORK", "pass", NULL);
        gchar *server_log = g_key_file_get_string(storage_server_config, "LOG", "server", NULL);

        strcpy(config->log_server,server_log);
        strcpy(config->wifi_ssid,wifi_ssid);
        strcpy(config->wifi_pass,wifi_pass);
    }
    else{

    }
}

void init_general_config(eraser_config_t *const config)
{
    config->log_server[0] = '\0';
    config->wifi_ssid[0] = '\0';
    config->wifi_pass[0] = '\0';
    
}



