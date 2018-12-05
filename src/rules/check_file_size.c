#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.c"

int main(int argc,char* argv[])
{
    char name[255];
    char cmd[255];

    int violation = 0;
    char * test;
    strcpy(name,argv[1]);

    FILE* size;

    strcpy(cmd,"stat -c \"%s\" ");
    strcat(cmd,name);

    size = popen(cmd, "r");

    int s = 0;

    if (fgets(name, sizeof(name), size) != NULL){
        test = strpbrk(name," ");
        if(test == NULL){
            s = atoi(name);
            if (s > atoi(argv[2])){
                violation = 1;
            }
        }else{
            return 0;
        }
    }

    if(violation){
        return 1;
    }else{
        return 0;
    }
}