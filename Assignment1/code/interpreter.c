#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"shellmemory.h"
#include"shell.h"
void shell(FILE* p);

int interpreter(char** commands,int num);

void help(){
    printf("\n");
    printf("help                Display all the commands\n");
    printf("quit                Exit / Terminates the shell with \"Bye!\n");
    printf("set VAR STRING      Assigns a value to shell memory\n");
    printf("print VAR           Display the string assgned to VAR\n");
    printf("run SCRIPT.TXT      Executes the file SCRIPT.TXT\n");
}
void quit(){
    printf("Bye!\n");
}
void set(char** words){  
    addVar(words);
}

void print(char** words){
    getValue(words[1]);
}

void run(char** args){

    char fileName[100];
    strcpy(fileName,args[1]);


    FILE* p = fopen(fileName,"rt");

    if(p == NULL){
        printf("Script not found.\n");
    }else{
        shell(p);
    }

    fclose(p);
}

void shell(FILE* p){
    char word[1000];     //Assume all input commands are no more than 1000 characters
    fgets(word,999,p);

    while(!feof(p)){
        int num = numOfArgs(word);       
        char** words = parse(word,num);     //words is the array for args
      
        int commandType = interpreter(words,num);
        if(commandType == 1){
            exit(1);//Terminates the shell
        }else if(commandType == -2){
            printf("Due to the syntax error, the script excution is terminated.\n");
            break;
        }
        fgets(word,999,p);
        if(feof(p)){
            break;
        }
    }
    
}

//commands -> an array of arguments
//num -> number of input arguments
int interpreter(char** commands,int num){

    char firstArg[100];
    strcpy(firstArg,commands[0]);
    //help
    if((strcmp(firstArg,"help") == 0)){
        if(num != 1){
            printf("Command does not exist, did you mean \"help\"?\n");
            return -2;
        }else{
            help();
        }
        return 0;
    //quit
    }else if((strcmp(firstArg,"quit")) == 0){
        if(num != 1){
            printf("Command does not exist, did you mean \"quit\"?\n");
            return -2;
        }else{
            quit();//Terminate the shell
        }
        return 1;
    //set
    }else if((strcmp(firstArg,"set") == 0)){

        if(num < 3){
            printf("Command does not exist, did you mean \"set VAR VALUE\"?\n");
            return -2;
        }else{
            set(commands);
        }
        return 0;
    //print
    }else if((strcmp(firstArg,"print") == 0)){
        if(num != 2){
            printf("Command does not exist, did you mean \"print VAR\"?\n");
            return -2;
        }else{
            print(commands);
        }
        return 0;
    //run
    }else if(strcmp(firstArg,"run") == 0){
        if(num != 2){
            printf("Command does not exist, did you mean \"run SCRIPT.txt\"?\n");
            return -2;
        }else{
            run(commands);
        }
        return 0;
    //unknown
    }else{
        printf("Unknown command\n");
        return -1;
    }
}
