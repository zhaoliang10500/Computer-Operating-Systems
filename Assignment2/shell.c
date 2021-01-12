#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"interpreter.h"
#include"ram.h"

int numOfArgs();
char** parse();
int lauchShell();
int freeCmd(char** arg,int num);

char** parse(char* str,int argNum){

    //Create a new array of strings
    char** args = (char**)malloc(sizeof(char*)*argNum);//Assumen each arguments has at most 100 chars
    for(int i = 0; i< argNum;i++){
        args[i] = (char*) malloc(sizeof(char)*100);
    }

    char word[100];
    memset(word,0,100);

    char* ptr = word;
    char temp;
    int length = strlen(str);
    int count = 0;
    
    int index = 0;
    while(str[index]==' '){
        index++;
    }
   
    for( int i = index; i<length && count < argNum; ){

        temp = str[i];

        if( temp == ' '|| temp == '\n' || temp == '\r' ) {
            strcpy(args[count], word);// temp[0] =  str;
			count++;
            //Empty the entire string
            memset(word,0,strlen(word));
            ptr = word;

            for( ; (temp==' ') && (i < length - 1) ; ) {
					i++;
					temp =  str[i];
				}
        }else{
            *ptr = temp;
            ptr++;
			i++;
        }  
    }
    return args;
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

int shellUI(int argc, char* argv[]){

    char* str1 = "\nKernel 1.0 loaded!\n"
                "Welcome to the <Liang Zhao> shell!\n"
                "Shell version 2.0 Updated Februrary 2020\n";
    printf("%s",str1);
    lauchShell();
    return 0;      
}

int lauchShell(){

    char arg[1000];//Assume all input commands are less than 1000 characters
    //memset(arg,' ',999);

    printf("%c ",'$');
    //fflush(stdout);
    fgets(arg,999,stdin);

    for(;1;){

        //If only '\n' is recieved, continue to prompt
        if(strcmp(arg,"\n") == 0){
            continue;
        }
        
        int num = numOfArgs(arg);//Returns num of input arguments
       
       //Input entire input cmd string, ane get the array back
        char** args = parse(arg,num);//Arguments array
        
        int code = interpreter(arg);//Launch the shell!

        if(code == 1){
            //freeCmd(args,num);//Free sapce allocated before!
            exit(0);//Terminates the shell
        }

        printf("%c ",'$');
        fgets(arg,999,stdin);

        if(feof(stdin)){
            FILE* p = freopen("/dev/tty", "r",stdin);
            fgets(arg,999,p);
            //printf("This is: %s",arg);
            continue;
        }
    }


    return 0;
}