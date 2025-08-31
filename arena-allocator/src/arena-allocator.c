#include "arena-defs.h" 
#include <string.h>
#include <unistd.h> 
#include <stdio.h> 
void initArena(void){
    if (arena_start == NULL) {
        arena_start = (void*)sbrk(ARENASIZE);
	if(arena_start == (void *)-1){
		perror("FAILED TO INIT THE ARENA"); 
	} 
        arena_current = arena_start;
        arena_end = arena_start + ARENASIZE;
    }
} 
void* allocArena(size_t size){
	initArena(); 
	size_t total_chunk_size = sizeof(Metadata) + size; 	// Leave room for Metadata 
	if(total_chunk_size % 16 != 0){
		total_chunk_size = ALIGN_TO_PAGE(total_chunk_size); // Align with 16 byte blocks
	}
	if(arena_end - arena_current > total_chunk_size){ // If free space is greater than what the user is asking for 
		void* this_alloc_end = arena_current + total_chunk_size; // Set a pointer to end and start of this allocation
		void* this_alloc_start = arena_current;
		arena_current = this_alloc_end; 
		Metadata* chunk_info = (Metadata*)this_alloc_start; // Fill Metadata
		chunk_info->size = total_chunk_size; 
		void* user_ptr = (char *)this_alloc_start + sizeof(Metadata); 
		return user_ptr; // Return pointer to start of chunk
	} 
	if(total_chunk_size > arena_end - arena_current){ // If not enough free space
 		void* newchunk_start = (void*)sbrk(total_chunk_size); // Get pointer to the start of the new chunk by asking OS for memory pages with sbrk
		if(newchunk_start == (void *)-1){
			perror("FAILED TO RESIZE ARENA"); 
		} 								     
	 	void* newchunk_current = newchunk_start;
	 	void* newchunk_end = (char *)newchunk_end + size; // As well as the end of the chunk 
		Metadata* newchunk_info = (Metadata *)newchunk_start;
		newchunk_info->size = size + sizeof(Metadata); // Fill Metadata 
		arena_current = newchunk_end;
		arena_end = (char *)newchunk_start + total_chunk_size;
		void* user_ptr = (char *)newchunk_start + sizeof(Metadata); 
		return user_ptr; // And return the pointer to the start of the new chunk
	}  
	return (void *)-1; 
} 
void freeArena(void* user_ptr){
	void* meta = (char *)user_ptr - sizeof(Metadata); // Decrement user pointer to go to metadata
	Metadata* chunk_info = (Metadata *)meta; //Cast to metadata pointer 
	size_t freelen = chunk_info->size;  // Create variable equal to size 
	arena_current = arena_current - freelen; // Deallocate said size by pushing back the "arena_current" pointer 
} 
void expandArena(size_t newsize){
	void* newmem = (void *)sbrk(newsize); // Just extend the arena by asking OS to extend the memory page 
	if(newmem != (void *)-1){
		arena_end = arena_end + newsize; // Push the end pointer further
	} 
} 
void resetArena(void){
	arena_current = arena_start; // Just Push the pointer back to the start (sbrk(0)) 
} 
void* callocArena(size_t size){
	void* user_ptr = allocArena(size);
	if (user_ptr != NULL){
		memset(user_ptr, 0, size);
	}
	return user_ptr;
}  
void* reallocArena(void* user_ptr, size_t newsize){	// NOTE: Finish realloc and other edge cases!!
	//void* meta = (char *)user_ptr - sizeof(Metadata);
	//Metadata* chunk_info = (Metadata *) meta; 
	//size_t toalloc = chunk_info->size - newsize;	
	//void* this_alloc_end = arena_current + toalloc;
	//return user_ptr; 
	return (void *)-1; 
} 
size_t freeBytes(void){
	return arena_end - arena_current; 
}  
size_t usedBytes(void){
	return arena_current - arena_start; 
}  
size_t totalBytes(void){
	return arena_end - arena_start; 
}  
