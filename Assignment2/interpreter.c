#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"shellmemory.h"
#include"shell.h"
#include"kernel.h"
#include"ram.h"

int interpreter(char* arg);

// 1 : scripts are loaded error(duplication)
// 2 : script not found error
// 3 : ram doesnot have space error.

static void help(){
    printf("\n");
    printf("help                    Display all the commands\n");
    printf("quit                    Exit / Terminates the shell with \"Bye!\n");
    printf("set VAR STRING          Assigns a value to shell memory\n");
    printf("print VAR               Display the string assgned to VAR\n");
    printf("run SCRIPT.TXT          Executes the file SCRIPT.TXT\n");
    printf("exec p1 p2 p3           Executes concurrent programs\n");
}

static int quit(){
    printf("%s\n","Bye!");
    return 1;
}

static void set(char** args){
    //If exists
    if(exist(args[1]) != -1 ){
        int index = exist(args[1]);//Get the index of the variable in the shell memory
        reset(args,index);
    //if does not exists
    }else{
        addVar(args);
    }

}

static void print(char** args){
    char* var = args[1];
    int index = exist(var);

    if(index != -1){
        printf("%s = %s\n",var,getValue(index));
    }else{
        printf("Variable \"%s\" does not exist.\n",var);
    }
}

void shell(FILE* p,char* fileName){

    char arg[1000];
    char buffer[100];//Used to get rid of unwanted chars
    fgets(arg,999,p);

    while(!feof(p)){
        int code = interpreter(arg);

        if(code == 1){
            break;
        }
        fgets(arg,999,p);
    }
}

static void run(char** args){
    char fileName[100];
    strcpy(fileName,args[1]);

    FILE* p = fopen(fileName,"rt");

    if(p == NULL){
        printf("Script %s not found.\n",fileName);
    }else{
        shell(p,fileName);
    }
    fclose(p);
}



int exec(char** args,int num){

    char* names[3];
    FILE* p[3];

    ///////////there is 1 script///////////
    if(num == 2){
        names[0] = args[1];
        p[0] = fopen(names[0],"rt");

        //if file does not exist
        if(p[0] == NULL){
            printf("Script %s not found.\n",names[0]);
            return 2;
        }else{
            fclose(p[0]);
            if(myinit(names[0])==3){
                initRam();
                return 0;
            } 
            scheduler();
            initRam();
            return 0;
        }

    //////////there are 2 scripts//////////
    }else if(num == 3){
        //two files are the same
        if(strcasecmp(args[1],args[2]) == 0){
            printf("Error:Script %s already loaded.\n",args[1]);
            return 1;
        }else{
            p[0] = fopen(args[1],"rt");
            p[1] = fopen(args[2],"rt");
            names[0] = args[1];
            names[1] = args[2];
            //if either of the two files cannot be found, return with not found error
            if(p[0]==NULL){
                printf("Script %s not found.\n",names[0]);
                return 2;
            }else if(p[1]==NULL){
                printf("Script <%s> not found.\n",names[1]);
                return 2;
            //Both scripts can be found
            }else{
                //Add p[0] into the simulator
                fclose(p[0]);
                if(myinit(names[0])==3){
                    initRam();
                    return 0;
                } 

                //Add p[1] into the simulator
                fclose(p[1]);
                if(myinit(names[1])==3) {
                    initRam();
                    return 0;
                }

                scheduler();//Run loaded programs
                initRam();//Clear the ram
                return 0;//Success
            }

        }
        
    /////////////there are 3 scripts///////////////// 
    }else{
        p[0] = fopen(args[1],"rt");
        p[1] = fopen(args[2],"rt");
        p[2] = fopen(args[3],"rt");
        names[0] = args[1];
        names[1] = args[2];
        names[2] = args[3];

        //check if any of them are the same script
        int script12 = strcasecmp(names[0],names[1]);
        int script13 = strcasecmp(names[0],names[2]);
        int script23 = strcasecmp(names[1],names[2]);
        
        if( script12 == 0 || script13 == 0 || script23 == 0 ){
            if(script12 == 0){
                printf("Error: Script %s already loaded.\n",names[0]);
                return 1;
            }else if(script13==0){
                printf("Error: Script %s already loaded.\n",names[0]);
                return 1;
            }else{
                printf("Error: Script %s already loaded.\n",names[1]);
                return 1;
            }
        //if no duplicate, check if p0 p1 p2 exist
        }else{
            if(p[0] == NULL){
                printf("Script %s not found.\n",names[0]);
                return 2;

            }else if(p[1]==NULL){
                printf("Script %s not found.\n",names[1]);
                return 2;
                
            }else if(p[2]==NULL){
                printf("Script %s not found.\n",names[2]);
                return 2;

            }else{

                //Add p[0] into the silulation
                fclose(p[0]);
                if(myinit(names[0]) == 3) {
                    
                    initRam();
                    return 0;
                }

                //Add p[1] into the simulation
                fclose(p[1]);
                if(myinit(names[1]) == 3) {
                    
                    initRam();
                    return 0;
                }

                //Add p[2] into the simulation
                fclose(p[2]);
                if(myinit(names[2]) == 3) {
                    
                    initRam();
                    return 0;
                }

                scheduler();//Run loaded programs
                initRam();//Clear the RAM

                return 0;//Success
            }
            return 0;
        }
    }
}

//**
//**@param:commands --> an array of arguments as bahs commands
//**@param:num --> the number of inputs of the commands.
//**
int interpreter(char* arg){

    int num = numOfArgs(arg);
    char** commands = parse(arg,num);

    char first[100];
    strcpy(first,commands[0]);

    //help command
    if( (strcmp(first,"help") == 0)){
        if(num != 1){
            printf("Wrong input argument.\n");
        }else{
            help();
        }
        return 0;

    //quit command
    }else if((strcmp(first,"quit")) == 0){
        if(num != 1){
            printf("Wrong input argument.\n");// -2 -> syntax error
        }else{
            return quit();//Terminate the shell
        }
        return 0;//Special case
        
    //set command
    }else if((strcmp(first,"set") == 0)){
        if(num != 3){
            printf("Wrong number of arguments! \"set\" var value.\n");
            return -2;
        }else{
            set(commands);
        }
        return 0;

    //print command  
    }else if((strcmp(first,"print") == 0)){
        if(num != 2){
            printf("Wrong number of arguments! \"print\" expects one argument input.\n");
            return -2;
        }else{
            print(commands);
        }
        return 0;

    //run command
    }else if(strcmp(first,"run") == 0){
        if(num != 2){
            printf("Wrong number of arguments! \"run\" needs 1 script as input.\n");
            return -2;
        }else{
            run(commands);
        }
        return 0;

    //exec command
    }else if(strcmp(first,"exec") == 0){
        if((num != 2)&&(num != 3)&&(num != 4)){
            printf("Wrong number of arguments! \"exec\" needs 1 to 3 scripts as inputs.\n");
            return -2;
        }else{
           exec(commands,num);
        }
        return 0;//exec == 5
    //if command is unknown
    }else{
        printf("Unknown command\n");
        return -1;
    }
}



