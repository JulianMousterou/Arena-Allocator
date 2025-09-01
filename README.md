# Linear-Bump-Allocator
Bump allocator written in C including alloc(), calloc(), and free() in bump allocator version. Lightweight and great on performance. In the process of adding realloc() and more error handling. 
NOTE: reallocBump() has been added. Only use at the end of the bump, attempting to reallocate middle memory blocks will result in unwanted behavior. 
## Usage
    #include "bump-defs.h" 
    #include <stdio.h> 
    #include <string.h> 

    int main(){

        int* values1 = allocBump(1024); 
        memset(values1, 0, sizeof(values1));
        for(int i = 0; i < 1024; i++){
                values1[i]+=1;
                printf("%d\n", values1[i]); 
        } 

        // OR USE CALLOC TO ZERO IT OUT 

        int* values2 = callocBump(1024); 
        for(int i = 0; i < 1024; i++){
                values2[i]=2;
                printf("%d\n", values2[i]);
        } 

        // ALWAYS FREE LAST USED BLOCK FIRST 

        freeBump(values2);
        freeBump(values1); 
    } 
## Behavior 
A bump allocator will not let you realloc() or free() from the middle. Following LIFO, in the code example above, you will have to free() "values2" before freeing "values1" as "values2" is at the end of the memory pool. Though it might be impractical in some use cases, in others it proves as a fast alternative for malloc(). I look to make more advanced allocators in the future, and any feedback anyone could give is accepted and valued. 
