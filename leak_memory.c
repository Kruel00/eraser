//
// Created By: Javier Castorena
//
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static GList * leak_memory_data = NULL;

void * leak_memory_malloc(guint size, const gchar *file, gint line, const gchar *func)
{
    printf("==== BEGIN leak_memory_malloc =====\n");
    guint *temp;
    void *p = malloc(size);
    if(p == NULL)
    {
        fprintf(stderr,"@@@ leak_memory_malloc failed, %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct LeakMemoryRow* leak_memory_row = malloc(sizeof(struct LeakMemoryRow));
    memset(leak_memory_row, 0, sizeof(struct LeakMemoryRow));

    sprintf((char *)&leak_memory_row->file, "%s", file);
    sprintf((char *)&leak_memory_row->func, "%s", func);
    leak_memory_row->line = line;
    leak_memory_row->size = size;

    leak_memory_row->p_alloced_storage = p;
    leak_memory_data = g_list_append(leak_memory_data, leak_memory_row);

    printf ("\t++Memory Allocated\n\t\tADRESS[SIZE]: %p[%i]\n\t\tFILE: %s:%d\n\t\tFUNC: %s()\n", p, size, file, line, func);

    printf("==== END leak_memory_malloc =====\n");
    return p;
}

void leak_memory_free(void *p, const gchar *file, gint line, const gchar *func)
{
    printf("==== BEGIN leak_memory_free =====\n");
    gboolean found = FALSE;

    GList * i = leak_memory_data;
    for (; i != NULL; i = i->next)
    {
        struct LeakMemoryRow* leak_memory_row = (struct LeakMemoryRow*)i->data;
        if(leak_memory_row->p_alloced_storage == p)
        {
            found = TRUE;
            break;
        }
    }

    if(!found)
    {
        printf("\tleak_memory_free called on unknown -> %s:%d %s()\n", file, line, func);
    }
    else
    {
        struct LeakMemoryRow* leak_memory_row = (struct LeakMemoryRow*)i->data;
        printf( "\t--Memory free -> %s:%d %s() \n\t\tADRESS[SIZE]: %ld[%d]\n\t\tFILE: %s:%d\n\t\tFUNC: %s()\n", file, line, func, (guint64)leak_memory_row->p_alloced_storage, leak_memory_row->size, leak_memory_row->file, leak_memory_row->line, leak_memory_row->func);
        leak_memory_data = g_list_remove(leak_memory_data, leak_memory_row);
        free(leak_memory_row);
    }

    free(p);
    p = NULL;

    printf("==== END leak_memory_free =====\n");
}

void leak_memory_free_check(void)
{
    printf("==== BEGIN leak_memory_free_check =====\n");
    gint total_mem_alloced = 0;

    printf("\tLeak_memory_free_check checking free memory. Len: %d\n", g_list_length(leak_memory_data));
    for (GList * i = leak_memory_data; i != NULL; i = i->next)
    {
        struct LeakMemoryRow* leak_memory_row = (struct LeakMemoryRow*)i->data;
        if(leak_memory_row->p_alloced_storage != NULL)
        {
            printf( "\t~~Memory leak\n\t\tADRESS[SIZE]: %ld[%d]\n\t\tFILE: %s:%d\n\t\tFUNC: %s()\n", (guint64)leak_memory_row->p_alloced_storage, leak_memory_row->size, leak_memory_row->file, leak_memory_row->line, leak_memory_row->func);
            total_mem_alloced += leak_memory_row->size;
            free((void *)leak_memory_row->p_alloced_storage);
            leak_memory_row->p_alloced_storage = NULL;
        }
    }
    printf( "\tTotal Memory leaked: %d\n", total_mem_alloced);
    printf("==== END leak_memory_free_check =====\n");
}
