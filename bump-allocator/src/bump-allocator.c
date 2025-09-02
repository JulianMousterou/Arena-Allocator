#include "/home/shark/code/projects/linear-bump-allocator/include/bump-defs.h" 
#include <string.h>
#include <unistd.h> 
#include <stdio.h> 
void initBump(void){
    if (bump_start == NULL) {
        bump_start = (void*)sbrk(MEMSIZE); // Extend OS memory page by MEMSIZE (4096) bytes 
        if(bump_start == (void *)-1){
                perror("FAILED TO INIT THE MEMORY POOL"); 
        } 
        bump_current = bump_start; // Set current pointer to start of memory
        bump_end = (char*)bump_start + MEMSIZE; // And the end to the last byte of the bump
    }
} 
void* allocBump(size_t size){
        initBump(); 
        if(size>1073741824){
                perror("too large");
                return (void*)-1;
        }
        size_t total_chunk_size = sizeof(Metadata) + size;      // Leave room for Metadata 
        if(total_chunk_size % 16 != 0){
                total_chunk_size = ALIGN_TO_PAGE(total_chunk_size); // Align with 16 byte blocks
        }
        if(bump_end - bump_current > total_chunk_size){ // If free space is greater than what the user is asking for 
                void* this_alloc_end = (char*)bump_current + total_chunk_size; // Set a pointer to end and start of this allocation
                void* this_alloc_start = bump_current;
                bump_current = this_alloc_end; 
                Metadata* chunk_info = (Metadata*)this_alloc_start; // Fill Metadata
                chunk_info->size = total_chunk_size; 
                void* user_ptr = (char*)this_alloc_start + sizeof(Metadata); 
                return user_ptr; // Return pointer to start of chunk
        } 
        if(total_chunk_size > bump_end - bump_current){ // If not enough free space
                void* newchunk_start = (void*)sbrk(total_chunk_size); // Get pointer to the start of the new chunk by asking OS for memory pages with sbrk
                if(newchunk_start == (void *)-1){
                        perror("FAILED TO RESIZE"); 
                }                                                                    
                void* newchunk_current = newchunk_start;
                void* newchunk_end = (char *)newchunk_end + size; // As well as the end of the chunk 
                Metadata* newchunk_info = (Metadata *)newchunk_start;
                newchunk_info->size = size + sizeof(Metadata); // Fill Metadata 
                bump_current = newchunk_end;
                bump_end = (char *)newchunk_start + total_chunk_size;
                void* user_ptr = (char *)newchunk_start + sizeof(Metadata); 
                return user_ptr; // And return the pointer to the start of the new chunk
        }  
        return (void *)-1; 
} 
int is_valid_ptr(void* user_ptr) {
    if (user_ptr == NULL) return 0;
    if (bump_start == NULL) return 0;
    if ((char*)user_ptr < (char*)bump_start + sizeof(Metadata)) return 0;
    if ((char*)user_ptr >= (char*)bump_end) return 0;
    return 1;
}
void freeBump(void* user_ptr){
        if(is_valid_ptr(user_ptr) != 1) return;
        if(bump_start == NULL){
                perror("MEMORY POOL LEFT UNINITIALIZED, NEVER ALLOCATED?");
        }
        void* meta = (char *)user_ptr - sizeof(Metadata); // Decrement user pointer to go to metadata
        Metadata* chunk_info = (Metadata *)meta; //Cast to metadata pointer 
        if((char*)meta + chunk_info->size != bump_current){
                perror("CAN ONLY FREE MOST RECENT ALLOC");
        } 
        size_t freelen = chunk_info->size;  // Create variable equal to size 
        bump_current = bump_current - freelen; // Deallocate said size by pushing back the "bump_current" pointer 
} 
void expandBump(size_t newsize){
        void* newmem = (void *)sbrk(newsize); // Just extend the bump by asking OS to extend the memory page 
        if(newmem != (void *)-1){
                bump_end = (char*)bump_end + newsize; // Push the end pointer further
        } 
} 
void resetBump(void){
        bump_current = bump_start; // Just Push the pointer back to the start (sbrk(0)) 
} 
void* callocBump(size_t size){
        void* user_ptr = allocBump(size);
        if (user_ptr != NULL){
                memset(user_ptr, 0, size);
        }
        return user_ptr;
}
void* reallocBump(void* user_ptr, size_t newsize){// NOTE: CAN ONLY REALLOC AT THE END OF AN ALLOCATION / TAIL OF MEMORY BLOCK. 
        if (user_ptr == NULL){
                return (void*)-1;
        }
        void* meta = user_ptr - sizeof(Metadata);
        Metadata* chunk_info = (Metadata *)meta; 
        if((char*)meta + chunk_info->size != bump_current){
                perror("CAN ONLY REALLOC MOST RECENT ALLOC");
                return (void*)-1;
        } 
        if(bump_end-bump_current > newsize){ // If there's enough free bytes in the memory to fit the resize
                bump_current = (char*)bump_current + newsize;
                bump_end = bump_current; // Push the end pointer forward by (newsize) bytes 
                return user_ptr; 
        } 
        if(bump_end-bump_current < newsize){ // If there is not enough space to fit the resize
                void* newchunk_start = (void *)sbrk(newsize); // sbrk() (newsize) bytes 
                void* newchunk_end = (char*)newchunk_start + newsize; // Get pointer to the new end 
                bump_end = newchunk_end; // Set global bump_end to the new total size 
                return user_ptr; 
        } 
        return (void *)-1; // return (void*)-1 on fail 
} 
size_t freeBytes(void){
        return bump_end - bump_current; 
}  
size_t usedBytes(void){
        return bump_current - bump_start; 
}  
size_t totalBytes(void){
        return bump_end - bump_start; 
}  
