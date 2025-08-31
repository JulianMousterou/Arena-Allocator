#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H
#include <stddef.h> 
#define ARENASIZE 4096
#define ALIGN_TO_PAGE(x) ((x+15) & ~15)
typedef struct Metadata {
	size_t size; 
} Metadata;
static void* arena_start = NULL; 
static void* arena_current = NULL; 
static void* arena_end = NULL; 
void* allocArena(size_t size); 
void initArena(void); 
void freeArena(void* user_ptr); 
void expandArena(size_t newsize);
void resetArena(void); 
void* callocArena(size_t size);
size_t usedBytes(void);
size_t freeBytes(void); 
size_t totalBytes(void); 
#endif 
