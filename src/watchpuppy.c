// Author: Matthew Michaelis

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <argp.h>
#include <dirent.h>

#define STD_STR_LEN 500
#define MAX_RULES 255
#define ASSOCIATION_PATH "associations/"
#define RULE_PATH "rules/"
#define ACTION_PATH "actions/"
#define META_PATH "meta/"

char cwd[STD_STR_LEN];

typedef struct{
    int id;
    char* rule_command;
    char* action_command;
    unsigned int freq;   //In ms
} rule;

const char *argp_program_version = "Watchpuppy 0.1";
const char *argp_program_bug_address = "matthew.michaelis@ucdenver.edu";
static char doc[] = "A lightweight custom system monitoring tool.";
static char args_doc[] = "[COMMAND]";

static struct argp_option options[] = { 
    { "add", 'a', 0, 0, "Add a new rule."},
    { "remove", 'r', 0, 0, "Remove an old rule."},
    { "start", 's', 0, 0, "Restart all rule daemons."},
    { "stop", 't', 0, 0, "Stop all daemons."},
    { "list", 'l', 0, 0, "List active rules."},
    { 0 } 
};
static error_t parse_opt(int key, char *arg, struct argp_state *state);

static struct argp argp = { options, parse_opt, args_doc, doc };

struct arguments {
    int ADD_MODE, REMOVE_MODE, START_MODE, STOP_MODE, LIST_MODE;
};

char * itoa(int i);
void constructor(int ID,rule *r);
void destructor(rule* r);
void rm_nl(char* str);
void rule_print(rule r);
void listRules(rule* rules, int num_rules);
int writeRules(rule* rules,int num_rules);
int writeRules(rule* rules,int num_rules);
int comp_rules(const void* s1, const void* s2);
int getRules(rule* rules);
int addRuleAction(rule* nr,char* dir_path,char type);
void addDaemon(rule* rules, int *num_rules);
void stopDaemons();
void startDaemons(rule* rules, int num_rules);
void removeDaemon(rule* rules, int *num_rules);


static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
    case 'a': arguments->ADD_MODE = 1; break;
    case 'r': arguments->REMOVE_MODE = 1; break;
    case 's': arguments->START_MODE = 1; break;
    case 't': arguments->STOP_MODE = 1; break;
    case 'l': arguments->LIST_MODE = 1; break;
    case ARGP_KEY_ARG: return 0;
    default: return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

char * itoa(int i){
    static char str[16];
    snprintf(str, 16*sizeof(char), "%d", i);
    return str;
}

void constructor(int ID,rule *r){
    r->id = ID;
    r->rule_command = (char*)calloc(STD_STR_LEN+1,sizeof(char));
    r->action_command = (char*)calloc(STD_STR_LEN+1,sizeof(char));
    r->freq = 0;
}

void destructor(rule* r){
    free(r->rule_command);
    free(r->action_command);
    free(r);
}

// Remove newline from strings
void rm_nl(char* str){
    strtok(str, "\n");
}

void rule_print(rule r){
    printf("%d) %s\n\t%s\n",r.id,r.rule_command,r.action_command);
}

void listRules(rule* rules, int num_rules){
    for(int i = 0; i < num_rules; i++){
        rule_print(rules[i]);
    }
}

int writeRules(rule* rules,int num_rules){
    // Replace all rules with those within the cache
    char str[STD_STR_LEN];
    FILE *file;
    DIR *d;
    struct dirent *dir;
    //d = opendir(ASSOCIATION_PATH);
    d = opendir(ASSOCIATION_PATH);
    int has_files = 0;
    if (d){
        while ((dir = readdir(d)) != NULL && dir->d_name != NULL){
            if((strcmp(dir->d_name,".") && strcmp(dir->d_name,".."))){
                has_files = 1;
            }
        }
        if(has_files){
            strcpy(str,"exec rm -r ");
            strcat(str,ASSOCIATION_PATH);
            strcat(str,"*");
            system(str);
        }
        closedir(d);
    }else{
        printf("ERROR: ASSOCIATION DIR CAN'T BE ACCESSED!\n");
    }
    for(int i = 0; i < num_rules; i++){
        strcpy(str,ASSOCIATION_PATH);
        strcat(str,"r");
        strcat(str,itoa(i+1));
        file = fopen(str,"w");
        fprintf(file,"%s\n",rules[i].rule_command);
        fprintf(file,"%s\n",rules[i].action_command);
        fprintf(file,"%d\n",rules[i].freq);
        fclose(file);
    }
}

int comp_rules(const void* s1, const void* s2){
    rule *r1 = (rule*) s1;
    rule *r2 = (rule*) s2;
    return r1->id - r2->id;
}

int getRules(rule* rules){
    char str[STD_STR_LEN];
    char path[STD_STR_LEN];
    char *s_ptr;
    int i = 0;
    int temp = 0;

    strcpy(path,cwd);
    strcat(path,ASSOCIATION_PATH);

    FILE *fptr;
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d){
        while ((dir = readdir(d)) != NULL && dir->d_name != NULL){
            if((strcmp(dir->d_name,".") && strcmp(dir->d_name,".."))){
                strcpy(str,path);
                strcat(str,dir->d_name);
                fptr = fopen(str,"r");
                s_ptr = strtok(dir->d_name,"r");
                int temp = atoi(s_ptr);
                
                constructor(temp,&rules[i]);
                rule r = rules[i];

                if(fgets (str, STD_STR_LEN, fptr)!=NULL ) {
                    rm_nl(str);
                    strcpy(r.rule_command,str);
                }
                
                if(fgets (str, STD_STR_LEN, fptr)!=NULL ) {
                    rm_nl(str);
                    strcpy(r.action_command,str);
                }
                if(fgets (str, STD_STR_LEN, fptr)!=NULL ) {
                    temp = atoi(str);
                    rules[i].freq = temp;
                }
                
                i++;

                fclose(fptr);
            }
        }
        qsort(rules, i, sizeof(rule), comp_rules);
        for(int j = 0; j < i;j++){
            rules[j].id = j+1;
        }
        closedir(d);
    }else{
        printf("ERROR: ASSOCIATION DIR CAN'T BE ACCESSED!\n");
    }
    return i;
}

int addRuleAction(rule* nr,char* dir_path,char type){
    char *usr_in = NULL;
    char path[STD_STR_LEN];
    char str[STD_STR_LEN];
    char cmd[STD_STR_LEN];
    char type_n[10];
    if(type == 'r'){
        strcpy(type_n, "rule");
    }else if(type == 'a'){
        strcpy(type_n, "action");
    }

    FILE *fptr;
    size_t len = 0;
    ssize_t read;
    int tmp = 1;
    int val = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir(dir_path);
    char *rulenames[100];

    if (d){
        while ((dir = readdir(d)) != NULL && dir->d_name != NULL){
            if((strcmp(dir->d_name,".") && strcmp(dir->d_name,".."))){
            printf("%d) %s\n",tmp,dir->d_name);
            rulenames[tmp] = malloc(sizeof(char)*strlen(dir->d_name)+5);
            strcpy(rulenames[tmp],dir->d_name);
            tmp++;
            }
        }

        printf("%d) Custom command... [BE CAREFUL].\n",tmp);

        printf("Enter the number of the %s you'd like to use.\n",type_n);
        read = getline(&usr_in,&len,stdin);
        val = atoi(usr_in);
        if(0 >= val || val > tmp){
            printf("Invalid selection. Exiting...\n");
            return 1;
        }
        if(val == tmp){
            printf("Enter the full command you wish to use as the %s. ",type_n);
            if(type == 'r'){
                printf("Make sure it outputs a 0 in normal cases, but a 1 if a rule violation has occured.\n");
            }
            read = getline(&usr_in,&len,stdin);
            if(type == 'r'){
                strcpy(nr->rule_command,usr_in);
            }else{
                strcpy(nr->action_command,usr_in);
            }
        }else{
            strcpy(path,META_PATH);
            strcat(path,rulenames[val]);
            strcat(path,".meta");
            strcpy(cmd,"./");
            strcat(cmd,dir_path);
            strcat(cmd,rulenames[val]);
            fptr = fopen(path,"r");
            if(fptr != NULL){
                while(fgets (str, STD_STR_LEN, fptr)!=NULL ) {
                    if(strcmp(str,"NONE")==0){
                        break;
                    }
                    printf(str);
                    fgets(str, STD_STR_LEN, fptr);
                    rm_nl(str);
                    read = getline(&usr_in,&len,stdin);
                    strcat(cmd," ");
                    
                    if(strcmp(str,"INT") == 0){
                        int min;
                        int max;
                        fgets(str, STD_STR_LEN, fptr);
                        min = atoi(str);
                        fgets(str, STD_STR_LEN, fptr);
                        max=atoi(str);
                        val = atoi(usr_in);
                        if(min >= val || val >= max){
                            printf("Invalid selection. Out of range. Exiting...\n");
                            return 1;
                        }else{
                            strcat(cmd,itoa(val));
                        }
                    }
                    if(strcmp(str,"CHAR") == 0){
                        int num_opt;
                        int opt_found = 0;
                        fgets(str, STD_STR_LEN, fptr);
                        num_opt = atoi(str);
                        for(int i = 0; i < num_opt;i++){
                            fgets(str, STD_STR_LEN, fptr);
                            if(strcmp(usr_in,str) == 0){
                                strcat(cmd,"-");
                                strcat(cmd,usr_in);
                                opt_found = 1;
                            }
                        }
                        if(opt_found == 1){
                            printf("Invalid selection. No matching char. Exiting...\n");
                            return 1;
                        }
                    }
                    if(strcmp(str,"STRING") == 0){
                        strcat(cmd,"\"");
                        rm_nl(strcat(cmd,usr_in));
                        strcat(cmd,"\"");

                    }
                }
            if(type == 'r'){
                strcpy(nr->rule_command,cmd);
            }else{
                strcpy(nr->action_command,cmd);
            }
            fclose(fptr);
            }else{
                printf("No metadata found for this %s. Please enter your arguments now.\n",type_n);                
                read = getline(&usr_in,&len,stdin);
                strcat(cmd,usr_in);
                if(type == 'r'){
                    strcpy(nr->rule_command,cmd);
                }else{
                    strcpy(nr->action_command,cmd);
                }
            }
        }
    }
    return 0;
}

void addDaemon(rule* rules, int *num_rules){
    char *usr_in = NULL;
    char path[STD_STR_LEN];

    size_t len = 0;
    ssize_t read;
    int val;
    rule nr;
    constructor(*num_rules+1,&nr);

    strcpy(path,cwd);
    strcat(path,RULE_PATH);

    if(addRuleAction(&nr,path,'r') == 1){
        return;
    }

    strcpy(path,cwd);
    strcat(path,ACTION_PATH);

    if(addRuleAction(&nr,path,'a') == 1){
        return;
    }

    printf("How frequently should the rule be ckecked?(In seconds, integer only).\n");
    read = getline(&usr_in,&len,stdin);
    val = atoi(usr_in);
    if(0 > val){
        printf("Invalid selection. Exiting...\n");
        return;
    }
    nr.freq = val;

    constructor(*num_rules+1,&rules[*num_rules]);
    rules[*num_rules].rule_command = nr.rule_command;
    rules[*num_rules].action_command = nr.action_command;
    rules[*num_rules].freq = nr.freq;

    *num_rules = *num_rules + 1;
    printf("Rule has been added.\n");
    stopDaemons();
    writeRules(rules,*num_rules);
    //startDaemons(rules,*num_rules);
    return;
}

void stopDaemons(){
    char path[STD_STR_LEN];
    strcpy(path,cwd);
    strcpy(path,ASSOCIATION_PATH);
    strcat(path,"kill");
    fopen(path,"w");
    sleep(2);
    remove(path);
}

void startDaemons(rule* rules, int num_rules){
    const char* r_command = rules[0].rule_command;
    const char* a_command = rules[0].action_command;

    for(int i = 0; i < num_rules; i++){
        fflush(stdout);
        if(fork() == 0){
            if(fork() == 0){

                const char* r_command = rules[i].rule_command;
                const char* a_command = rules[i].action_command;
            
                while(1){
                    int sleep_counter = 0;

                    while(sleep_counter < rules[i].freq){
                        sleep(1);
                        sleep_counter++;
                        char path[STD_STR_LEN];
                        strcpy(path,ASSOCIATION_PATH);
                        strcat(path,"kill");
                        if( access(path, F_OK ) != -1 ) {
                            exit(0);
                        }
                    }
                    FILE *s_call = popen(r_command,"r");

                    int exit = WEXITSTATUS(pclose(s_call));
                    if (exit == 1){
                        s_call = popen(a_command,"r");
                        pclose(s_call);
                    }
                }
                exit(0);
            }else{
                exit(0);
            }
        }
    }
}

void removeDaemon(rule* rules, int *num_rules){
    char *usr_in = NULL;
    size_t len = 0;
    ssize_t read;
    int val = 0;
    char str[STD_STR_LEN];

    listRules(rules, *num_rules);
    printf("Enter the number of the daemon you'd like to remove.\n");
    
    read = getline(&usr_in,&len,stdin);
    val = atoi(usr_in);
    if(0 >= val || val > *num_rules){
        printf("Invalid selection. Exiting...\n");
        return;
    }

    DIR *d;
    struct dirent *dir;
    d = opendir(ASSOCIATION_PATH);
    if (d){
        strcpy(str,"exec rm -r ");
        strcat(str,ASSOCIATION_PATH);
        strcat(str,"r");
        strcat(str,itoa(val));
        system(str);
    }else{
        printf("ERROR: ASSOCIATION DIR CAN'T BE ACCESSED!");
    }
    closedir(d);
    *num_rules--;
    printf("Resetting Daemons....\n");
    getRules(rules);
    stopDaemons();
    startDaemons(rules,*num_rules);
    printf("Daemon deleted.\n");
}

int main (int argc, char **argv){
    size_t MAX_LEN = 500*sizeof(char);
    //getcwd(cwd,MAX_LEN);
    strcpy(cwd,"");
    struct arguments args;
    args.ADD_MODE = 0;
    args.REMOVE_MODE = 0;
    args.START_MODE = 0;
    args.STOP_MODE = 0;
    args.LIST_MODE = 0;

    argp_parse (&argp, argc, argv, 0, 0, &args);

    rule rules[MAX_RULES];

    int num_rules = getRules(rules);
    char str[STD_STR_LEN];

    if(args.ADD_MODE){
        addDaemon(rules, &num_rules);
    }else if(args.REMOVE_MODE){
        removeDaemon(rules, &num_rules);
    }else if(args.START_MODE){
        startDaemons(rules,num_rules);
    }else if(args.STOP_MODE){
        stopDaemons();
        printf("All daemons stopped.\n");
    }else if(args.LIST_MODE){
        listRules(rules,num_rules);
    }
    return 0;
}