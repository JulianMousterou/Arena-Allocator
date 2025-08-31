# Linear-Bump-Allocator
Bump allocator written in C including alloc(), calloc(), and free() in bump allocator version. Lightweight and great on performance. In the process of adding realloc() and more error handling. 
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
