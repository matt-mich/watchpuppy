#include <stdio.h>
#include <stdlib.h>

int main()
{
    char original[10000]={'\0'};
    char target[10000]={'\0'};
    char command[20100]={'\0'};
    printf("Enter the file path you want to move.\n");
    scanf("%s",original);
    printf("Enter the target file path you want to move in .\n");
    scanf("%s",target);
    sprintf(command,"mv %s %s",original,target);
    system(command);
    return 0;
}
