//
// Created By: Javier Castorena
//
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <json-glib/json-glib.h>
#include "common.h"

char *LOGIN_URL = "https://intranet.valuout.com/ToolsWebsite/api/CloverTestWeb/MobileLogin";
char *POST_URL = "https://intranet.valuout.com/ToolsWebsite/api/CloverTestWeb/SaveTestResults";

void recive_info(){
    printf("This is a call from main\n");
}

void show_process_devices(){
    printf("log processeced devices\n");
}

void init_erase_device_log(erase_device_log_t * const erase_device_log){

    erase_device_log->name[0] = '\0';
    erase_device_log->sys_path[0] = '\0';
    erase_device_log->serialNumber[0] = '\0';
	erase_device_log->model[0] = '\0';
	erase_device_log->vendor[0] = '\0';
	erase_device_log->bus[0] = '\0';
	erase_device_log->usb_driver[0] = '\0';
	erase_device_log->type_attribute[0] = '\0';
	erase_device_log->sg_name[0] = '\0';
	erase_device_log->nvme_short_name[0] = '\0';
    erase_device_log->capacity[0] = '\0';
    erase_device_log->erase_status = 0;
}

void json_save(storage_device_t erase_info){

gchar json_log_file_name[1024];

GError *glib_error = NULL;

gchar storage_device_capacity_gb[32];
const long double gb_koeff = (1000.0 * 1000.0) * 1000.0;

  const gchar *erase_state[] = {"Idle", "Erase Sussess", "Erase Pass"," Erasing"};

  JsonBuilder *json_builder = json_builder_new();
  json_builder_begin_object(json_builder);

    json_builder_set_member_name(json_builder, "validateTicket");
    json_builder_add_boolean_value(json_builder,false);

    json_builder_set_member_name(json_builder,"generateTicket");
    json_builder_add_boolean_value(json_builder,false);
    
    json_builder_set_member_name(json_builder,"receiveOnly");
    json_builder_add_boolean_value(json_builder,true);

    json_builder_set_member_name(json_builder,"customerCode");
    json_builder_add_null_value(json_builder);

    json_builder_set_member_name(json_builder,"hwid");
    json_builder_add_null_value(json_builder);

    json_builder_set_member_name(json_builder,"cpu");
    json_builder_add_null_value(json_builder);

    json_builder_set_member_name(json_builder,"mlb");
    json_builder_add_null_value(json_builder);

    json_builder_set_member_name(json_builder,"brandCode");
    json_builder_add_null_value(json_builder);

    json_builder_set_member_name(json_builder,"arch");
    json_builder_add_null_value(json_builder);

    json_builder_set_member_name(json_builder,"sku");
    json_builder_add_null_value(json_builder);
    
    json_builder_set_member_name(json_builder, "serialNumber");
    json_builder_add_string_value(json_builder, erase_info.serial);  

    json_builder_set_member_name(json_builder,"identifier");
    json_builder_add_string_value(json_builder,"EraseModule");

    json_builder_set_member_name(json_builder,"username");
    json_builder_add_string_value(json_builder, "");

    json_builder_set_member_name(json_builder,"assetId");
    json_builder_add_null_value(json_builder);

    json_builder_set_member_name(json_builder,"version");
    json_builder_add_null_value(json_builder);

    json_builder_set_member_name(json_builder,"memory");
    json_builder_add_null_value(json_builder); 
    
    g_snprintf(storage_device_capacity_gb, G_N_ELEMENTS(storage_device_capacity_gb), "%.2Lf GB", (long double)(erase_info.sector_size * erase_info.total_sectors)/ gb_koeff);
    json_builder_set_member_name(json_builder,"storage");
    json_builder_add_int_value(json_builder,1+((erase_info.sector_size * erase_info.total_sectors)/ gb_koeff));
    
    json_builder_set_member_name(json_builder,"area");
    json_builder_add_null_value(json_builder);
    
    json_builder_set_member_name(json_builder,"testResults");
    json_builder_begin_object(json_builder);    
    json_builder_end_object(json_builder);

    json_builder_set_member_name(json_builder,"vpd");
    json_builder_begin_object(json_builder);

        json_builder_set_member_name(json_builder, "DevicePath");
        json_builder_add_string_value(json_builder, erase_info.name);  

        json_builder_set_member_name(json_builder, "Vendor");
        json_builder_add_string_value(json_builder, erase_info.vendor);   

        json_builder_set_member_name(json_builder, "Capacity");
        json_builder_add_string_value(json_builder, storage_device_capacity_gb);

        json_builder_set_member_name(json_builder, "BusType");
        json_builder_add_string_value(json_builder, erase_info.bus);

        json_builder_set_member_name(json_builder, "EraseResult");
        json_builder_add_string_value(json_builder, "success");

    json_builder_end_object(json_builder);
    json_builder_set_member_name(json_builder,"findings");
    json_builder_begin_array(json_builder);
    json_builder_end_array(json_builder);

  json_builder_end_object(json_builder);

  JsonGenerator *json_generator = json_generator_new();
  JsonNode *json_root = json_builder_get_root(json_builder);

  json_generator_set_pretty(json_generator, TRUE);
	json_generator_set_root(json_generator, json_root);

  g_snprintf(json_log_file_name, G_N_ELEMENTS(erase_info.serial), "%s.json",erase_info.serial);
  json_generator_to_file(json_generator, json_log_file_name, &glib_error);

}

void init_erase_device_log_list(erase_device_log_list_t * const erase_device_log){
    erase_device_log->erase_log = NULL;
    erase_device_log->count = 0U;
}

void free_erase_device_log_list(erase_device_log_list_t * const erase_device_log) {

	for(size_t i = 0U; i < erase_device_log->count; ++i) {
		init_erase_device_log(&erase_device_log->erase_log[i]);
	}
	free(erase_device_log->erase_log);
	erase_device_log->erase_log = NULL;

	erase_device_log->count = 0U;
}

erase_device_log_list_t show_list_devices_log(){
    
    erase_device_log_t erase_device_log;
    erase_device_log_list_t list_device_logs;
    
    return list_device_logs;
}   

void append_erase_log(erase_device_log_t * const erase_log){
    
}


int upload_erase_test_result(storage_device_t erase_info)
{
    gchar json_log_file_name[1024];
    g_snprintf(json_log_file_name, G_N_ELEMENTS(erase_info.serial), "%s.json", erase_info.serial);

    // https://stackoverflow.com/questions/3747086/reading-the-whole-text-file-into-a-char-array-in-c

    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen(json_log_file_name, "rb");
    fseek(fp, 0L, SEEK_END);
    lSize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    buffer = (char*)calloc(lSize, sizeof(char));   
    fread(buffer, sizeof(char), lSize, fp);
    

    char *token;
    if(login(&token))
    {
        //error
        fputs("error performing login",stderr);
        free(buffer);
        free(token);
        return -1;
    }

    if(post_result(buffer,token))
    {
        //err
        fputs("error while posting result",stderr);
        free(buffer);
        free(token);
        return -1;
    }

    free(buffer);
    free(token);
}


int login(char** token)
{
    char command[1024];
    char * cmd_ptr = command;
    
    cmd_ptr += sprintf( cmd_ptr, "curl --location --request POST '%s' --header 'Content-Type: application/json' --data-raw '{\"username\":\"\",\"password\":\"\",\"serialNumber\":\"\",\"assetID\":\"\",\"validateTicket\":\"false\"}'",LOGIN_URL);


    FILE *command_output;
    
    char line[1024];
    char response_buffer[2048];

    memset(line,'\0',1024);
    memset(response_buffer,'\0',2048);

    /* Open the command for reading. */
    command_output = popen(command, "r");


    if (command_output == NULL)
    {
        printf("Failed to run command\n");
        exit(1);
    }

    char *response = response_buffer;


    /* Read the output a line at a time - output it. */
    while (fgets(line, sizeof(line), command_output) != NULL)
    {
        //printf("%s", line);
        response += sprintf(response,"%s", line);
        memset(line,'\0',1024);
    }

    char * token_ptr = strstr(response_buffer, "token");

    if(!token_ptr)
    {
        //failed
        fprintf(stderr,"token not found\n");
        return -1;
    }

    char * collon_ptr = strchr(token_ptr,':') + 1;
    char* first_double_quote = strchr(collon_ptr, '"') + 1;
    char* second_double_quote = strchr(first_double_quote,  '"');
    *second_double_quote = 0;
    int length = second_double_quote - first_double_quote;
    char * out_string = malloc(sizeof(char) * length);
    sprintf(out_string, "%s", first_double_quote);
    *token = out_string;
    
    return 0;
}


int post_result(char* json_content, char* token)
{
    /* do your work here, buffer is a string contains the whole text */

    char cmdBuffer[2048];

    char *command_ptr = cmdBuffer; // starts at init
    
    command_ptr += sprintf(command_ptr, "curl --location --request POST '%s' ", POST_URL);
    command_ptr += sprintf(command_ptr, "--header 'Content-Type: application/json' ");
    command_ptr += sprintf(command_ptr, "--header 'Authorization: Bearer %s' ",token);
    command_ptr += sprintf(command_ptr, "--data-raw '%s'", json_content);
    
    FILE *commandFile;
    char line[1035];
    char response[1024];
    char *response_ptr = response;
    /* Open the command for reading. */
    commandFile = popen(cmdBuffer, "r");
    if (commandFile == NULL)
    {
        printf("Failed to run command\n");
        exit(1);
    }

    /* Read the output a line at a time - output it. */
    while (fgets(line, sizeof(line), commandFile) != NULL)
    {
        response_ptr += sprintf(response_ptr,"%s",line);
    }

    printf("\nRespuesta:  %s\n",line);

    /* close */
    pclose(commandFile);

    return 0;
}
