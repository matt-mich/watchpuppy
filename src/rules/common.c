#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int getPIDfromName(char* name,int* pid_arr, int* length){

    FILE *pid;
    char path[255];
    char *pid_val;
    
    strcpy(path,"pidof ");
    strcat(path,name);

    pid = popen(path, "r");
    int counter = 0;
    if (fgets(path, sizeof(path), pid) != NULL){
        printf(path);

        pid_val = strtok(path," ");
        while(pid_val != NULL){
            pid_arr[counter] = atoi(pid_val);
            counter++;
            pid_val = strtok (NULL," ");
        }
        pclose(pid);
        *length = counter;

        return 0;
    }
}


unsigned int timeToMinutes(char* time){
	char *split;
	unsigned int minutes = 0;
	split = strtok(time,":");
	minutes += atoi(split)*1440;
	split = strtok(NULL,":");
	minutes += atoi(split)*60;
	split = strtok(NULL,":");
	minutes += atoi(split);
	return minutes;
}


float getUtilFromPID(int pid,int type){

	// Type:
	// 1........ % CPU Utilization
	// 2........ % Memory Utilization
    FILE *pid_util;
    char path[255];
    char pid_str[20];
    float utilization = 0;

    sprintf(pid_str,"%d",pid);

    strcpy(path,"/bin/ps -p ");
    strcat(path,pid_str);
	switch(type){
		case(0):
		    strcpy(pid_str," -o %cpu");
			break;
		case(1):
		    strcpy(pid_str," -o %mem");
			break;
		case(2):
		    strcpy(pid_str," -o time");
			break;
		default:
		    strcpy(pid_str," -o %cpu");
			break;
	}
    strcat(path,pid_str);
    pid_util = popen(path, "r");
    
    if (pid_util == NULL) {
        printf("ps error \n" );
        return(0.0);
    }

    fgets(path, sizeof(path)-1, pid_util);

	if(type != 2){
		if(fgets(path, sizeof(path)-1, pid_util)){
			utilization = atof(path);
		}
	}else{
		if(fgets(path, sizeof(path)-1, pid_util)){
			unsigned int time = timeToMinutes(path);
			utilization += time;
		}
	}
    
    return utilization;
}