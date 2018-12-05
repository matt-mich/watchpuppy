#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("Enter the path.\n");
    char path[10000]={'\0'};
    char commond[10000]={'\0'};
    scanf("%s",path);
    sprintf(commond,"cat %s",path);
    system(commond);
    return 0;
}

