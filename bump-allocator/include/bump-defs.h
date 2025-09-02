#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H
#include <stddef.h> 
#define MEMSIZE 4096
#define ALIGN_TO_PAGE(x) ((x+15) & ~15)
typedef struct Metadata {
        size_t size; 
} Metadata;
static void* bump_start = NULL; 
static void* bump_current = NULL; 
static void* bump_end = NULL; 
int is_valid_ptr(void* user_ptr);
void initBump(void); 
void* allocBump(size_t size); 
void* callocBump(size_t size);
void* reallocBump(void* user_ptr, size_t newsize);
void freeBump(void* user_ptr); 
void expandBump(size_t newsize);
void resetBump(void); 
size_t usedBytes(void);
size_t freeBytes(void); 
size_t totalBytes(void); 
#endif 
