#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.c"


int main(int argc,char* argv[])
{
    int pid_len = 100;
    int *pid = malloc(40 * pid_len);

    int length;
    char name[128];
    int violation = 0;

    if(argc != 3){
        return 0;
    }

    strcpy(name,argv[1]);
    getPIDfromName(name,pid,&length);

    float sum = 0.0;
    for(int i = 0; i < length; i++){
        sum += getUtilFromPID(pid[i],0);
    }
    if(sum > atof(argv[2])){
        violation = 1;        
    }
    
	free(pid);

    if(violation){
        return 1;
    }else{
        return 0;
    }
}