#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"ram.h"

int nextAvailableCell = 0;

//Instantiate the RAM
void initRam(){
    for(int i = 0; i< 40; i++){
        ram[i] = NULL;
    }
    nextAvailableCell = 0;
}

//Return one instruction from ram[index]
char* getLineOfInstruction(int index){
    return ram[index];
}

int addToRAM(FILE* p, int* start, int* end){
    char buffer[100];
    *start = nextAvailableCell;
    int index = *start;

    fgets(buffer,99,p);

    while(!feof(p)){
        if (index < 1000) {
            ram[index] = strdup(buffer);//Memory dynamically allocated by the OS
            index++;

            fgets(buffer,99,p);
        }
        else {
            printf("Error: Not enough space in RAM to load your program(s).\n");
            return 3;
        }
    }

    nextAvailableCell = index;
    //store the end index
    *end = (index-1);

    return 0;
}

void clearRAM(int start,int end){
    for(int i = start; i <= end; i++){
        ram[i] = NULL;
    }
}

int RamNextAvailableIndex(){
    return nextAvailableCell;
}
