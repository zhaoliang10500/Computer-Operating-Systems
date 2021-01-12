#include<stdlib.h>
#include<stdio.h>
#include<string.h>

struct MEM{
    char var[100];//Assume each var or value can have at most 100 chars
    char value[100];
};

struct MEM mems[50];//Assume that there will be at most 50 variables in the shell memory at once
int count = 0;//Keep track of number of vars in the shell memory

//Check if the variable already exists in the shell memory
//if exists return the index
//if not exist return -1
int exist(char* var){
    for(int i =0; i<50; i++){
        //printf("%s %s\n",temp,mems[i].var);
        if(strcmp(var,mems[i].var) == 0){
            return i;
        }
    }
    return -1;
}

//Add one variable into the shell memory
void addVar(char** words){
    int valIndex = exist(words[1]);
    if (valIndex == -1) {
        strcpy(mems[count].var,words[1]);
        strcpy(mems[count].value,words[2]);
        count = count + 1;//Increment memory by one
    }
    else {
        strcpy(mems[valIndex].value,words[2]);
    }
}

void getValue(char* var){
    int valIndex = exist(var);
    if (valIndex == -1) {
        printf("Variable does not exist\n");
    }
    else {
        printf("%s\n", mems[valIndex].value);
    }
}