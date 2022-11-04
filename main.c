//
// Created By: Javier Castorena
//
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#define DEBUG

#define YES -8
#define NOT -9

#ifdef DEBUG
    void show_message(){
        //show_debug_dialog();
        printf("DEBUG MODE\n");
    }
    bool debug_mode = true;
#else
    void show_message(){
        printf("DEBUG MODE IS DISABLED");
        if(argc <= 3)
        show_help();
    }

    bool debug_mode = false;
#endif

int main(int argc, char** argv)
{
    gtk_init(&argc, &argv);
    show_message();
    
    //config.ini values
    eraser_config_t general_config;
    init_general_config(&general_config);
    load_config(&general_config);


    printf("Server: %s\n",general_config.log_server);
    
    /*
    //Message box 
    int confirm = erase_confirm();
    if(confirm == NOT){
        printf("NO SE BORRA");
    }
    else{
        confirm = erase_confirm();
        if(confirm == YES)
            printf("BORRANDO DISCO..");
    }*/
}

