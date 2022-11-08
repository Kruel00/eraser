//
// Created By: Javier Castorena
//
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

//#define DEBUG

#define YES -8
#define NOT -9

#ifdef DEBUG
    void show_message(){
        printf("Mode: ");
        printf("\033[0;34m");
        printf("DEBUG\n");
        printf("\033[0m");
        
    }
    bool debug_mode = true;
#else
    void show_message(int argc, char** argv){
        printf("Mode: ");
        printf("\033[0;32m");
        printf("RELEASE\n");
        printf("\033[0m");
        if(argc < 2){
            show_help();
            exit(1);
        }
    }

    bool debug_mode = false;
#endif

void show_test_result(char msg[64], int t);

storage_device_list_t device_list;
storage_device_t working_device;
bool load_device_info = false;
enum{
        SATA,
        USB,
        NVME,
        MMC
    }; 


int main(int argc, char** argv)
{

    char *device_serial;
    gtk_init(&argc, &argv);
    show_message(argc, argv);
    int confirm;

    //config.ini values
    eraser_config_t general_config;
    init_general_config(&general_config);
    load_config(&general_config);

    printf("numero de argumentos: %i\n",argc);

    if (detect_storage_devices(&device_list) == 0)
    {   
        show_test_result("Load devices: ", 0);
    }
    else{
        show_test_result("Load devices: ", 1);
    }

    find_device_by_name(device_list, &working_device, argv[1]);

    //device
    printf("Device: %s\n",argv[1]);
    if(strlen(working_device.name) < 1){
        show_test_result("Load storage info: ", 1);
        alert("Device not found!");
        exit(1);
    }
    else{
        show_test_result("Load storage info: ", 0);
        load_device_info = true;
    }

    if(load_device_info){
        printf("Device type = %i\n",working_device.type);
    }
    
    if(load_device_info){
        switch (working_device.type)
        {
        case SATA:
            printf("SATA\n");
                confirm = erase_confirm();
                printf("Respuesta: %i",confirm);
            break;
        case USB:
            printf("USB\n");
            if(erase_confirm() == YES)
                if(erase_confirm() == YES){
                     printf("BORRAR\n");
                     erase_sg_device(working_device);
                }
            break;
        case NVME:
            printf("NVME");
        break;
        case MMC:
        if(erase_confirm() == YES)
                if(erase_confirm() == YES){
                     printf("BORRAR\n");
                     erase_sg_device(working_device);
                }
            //printf("MMC");
        break;
        default:
            printf("NO COMPATIBLE");
        break;  
    }
    }
}

void show_test_result(char msg[64], int t){
    printf("%s",msg);

    if(t  == 0)
    {
        printf("\033[0;34m");
        printf("PASS\n");
        printf("\033[0m");
    }
    else{
        printf("\033[0;31m");
        printf("FAIL\n");
        printf("\033[0m");
    }
}   