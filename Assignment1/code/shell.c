#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"interpreter.h"

//Parse the input string into an array of strings
char** parse(char* str,int argNum){

    //Create an array of strings with length of 100 chars
    char** words = (char**)malloc(sizeof(char*)*argNum);
    for(int i = 0; i< argNum;i++){
        words[i] = (char*) malloc(sizeof(char)*100);
    }

    char word[100];
    memset(word,0,100);
    int count = 0;
    int i = 0, j = 0;
    //skip white spaces in the front
    for ( ; (str[i]==' ') && (i < strlen(str) - 1); i++);
    while(i < strlen(str) && count < argNum){
        //if it is at the end of this string, copy it to words
        if(str[i] == '\n' || str[i] == ' ' || str[i] == '\0' || str[i] == '\r') {
            strcpy(words[count], word);
			count++;
            memset(word,0,strlen(word));
            j = 0;
            //skip the rest white spaces
            for ( ; (str[i]==' ') && (i < strlen(str) - 1); i++);
        //else go to the next char
        }else{
            word[j] = str[i];
			i++, j++;
        }  
    }
    return words;
}

//Returns number of input arguments
int numOfArgs(char* str){

    int count = 1;
    char tempChar;
    for(int i  = 0; i < strlen(str);i++){
        tempChar = *(str+i);
        if(tempChar == ' '){
            count++;
            //skip the rest white spaces
            for( ; (tempChar == ' ') && (i < strlen(str) - 1);i++,tempChar =  str[i]);
        }
    }
    return count;
}

int main(int argc, char* argv[]){

    printf("\nWelcome to the Liang Zhao shell!\n");
    printf("Version 1.0 Created January 2020\n");

    char word[1000];     //Assume all input commands are no more than 1000 characters
    char buffer[100];   //Used to get rid of unwanted chars
    
    while(1){
        printf("$ ");
        fgets(word,999,stdin);

        //If only '\n' is recieved, continue to prompt
        if(strcmp(word,"\n") == 0){
            continue;
        }

        int num = numOfArgs(word);//Returns num of input arguments
        char** words = parse(word,num);//Arguments array
        
        int commandType = interpreter(words,num);

        if(commandType == 1){
            exit(0);//Terminates the shell
        }
    }
}


