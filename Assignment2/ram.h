#include<stdio.h>


//extern char* ram[1000];//1000 cells of memory locations

extern int nextAvailable;//the next available cell in RAM

//Initiate the RAM
void initRam();

int addToRAM(FILE* p,int* star, int* end);

char* getLineOfInstruction(int index);

void clearRAM(int start,int end);

int RamNextAvailableIndex();