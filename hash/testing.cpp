#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

int main()
{
    int x = 0;
    
    for (int i = 0; i < 10; i++)
        printf("%d\n", i);
    
    printf("DEBUG");
    
    char* mem = (char*)malloc(10);
  
    return 0;
}