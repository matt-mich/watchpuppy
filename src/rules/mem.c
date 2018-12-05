#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.c"

int main(int argc,char* argv[])
{

    // Rule Types
    // -u ...... Total RAM utilization of all processes

    int *pid;
    int length;
    char default_name[20];
    char default_param[3];
    char default_val[5];
    char name[128];
    int violation = 0;

    if(argc != 4){
		printf("Invalid parameter");
		return -1;
	}


	/*
    if(argc != 4){
        strcpy(default_name,"chromium");
        strcpy(default_param,"-u");
        strcpy(default_val,"5");
        argc = 4;
        argv[1] = default_name;
        argv[2] = default_param;
        argv[3] = default_val;
    }
	*/

    strcpy(name,argv[1]);
    getPIDfromName(name,&pid,&length);

	//printf("%d Associated Processes \n", length);

	if(strcmp(argv[2],"-u")){
		int pid_len = length;
		float sum = 0.0;
		for(int i = 0; i < pid_len; i++){
			float temp = getUtilFromPID(pid[i],1);
			sum += temp;
		}

		printf("%.2f%% RAM Consumption \n",sum);

        violation = sum < atof(argv[3]);
	}else{
		printf("Invalid parameter");
	    return -1;
	}
	free(pid);
	if(violation){
        printf("Rule violation detected.\n");
        return 1;
    }else{
        printf("No rule violation detected.\n");
        return 0;
    }
}