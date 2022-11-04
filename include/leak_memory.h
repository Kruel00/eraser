#ifndef LEAK_MEMORY_H
#define LEAK_MEMORY_H

#include <stdio.h>

#ifdef DEBUG
    #define MALLOC(pointer) leak_memory_malloc( pointer , __FILE__, __LINE__, __FUNCTION__)
    #define FREE(pointer) if ( pointer != NULL) leak_memory_free( pointer , __FILE__, __LINE__, __FUNCTION__);
    #define FREE_CHECK() leak_memory_free_check()
#else
    #define MALLOC(pointer) malloc(pointer);
    #define FREE(pointer) free(pointer);
    #define FREE_CHECK()
#endif // DEBUG

struct LeakMemoryRow
{
    gchar file [512];
    gint line;
    gchar func [128];
    guint64 *p_alloced_storage;
    gint size;
};

void * leak_memory_malloc(guint size, const gchar *file, gint line, const gchar *func);
void leak_memory_free(void *p, const gchar *file, gint line, const gchar *func);
void leak_memory_free_check(void);

#endif //LEAK_MEMORY_H
