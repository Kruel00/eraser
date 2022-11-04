//
// Created By: Javier Castorena
//
#include <glib.h>


typedef struct erase_device_log_s
{
    bool validateTicket;
    bool generateTicket;
    bool receiveOnly;
    gchar customerCode[128];
    gchar hwid;
    gchar cpu;
    gchar mlb;
    gchar brandCode;
    gchar arch[256];
    gchar sku[256];
    gchar identifier[256];
    gchar username[256];
    gchar assetId[256];
    gchar version[256];
    gchar area[256];
    gint memory;
    gint storage;
    gchar name[32];
    gchar sys_path[512];
    gchar vendor[256];
    gchar serialNumber[256]; 
	gchar model[128];
	gchar bus[32];
	gchar usb_driver[32];
	gchar type_attribute[32];
	gchar sg_name[32];
	gchar nvme_short_name[32];
    gchar capacity[256];
    gint  erase_status;
}erase_device_log_t;


typedef struct erase_device_log_list_s{
    erase_device_log_t *erase_log;
    size_t count;
}erase_device_log_list_t;

enum {
    IDLE,
    ERASE_PASS,
    ERASE_FAIL,
    ERASING
};

void recive_info();
void show_process_devices();
void init_erase_device_log(erase_device_log_t * const erase_device_log);
void init_erase_device_log_list(erase_device_log_list_t * const erase_device_log_list);
erase_device_log_list_t show_list_devices_log();
void free_erase_device_log_list(erase_device_log_list_t * const erase_device_log);
void json_save(storage_device_t erase_info);
int upload_erase_test_result(storage_device_t erase_info);
int login(char** token);
int post_result(char* json_content, char* token);