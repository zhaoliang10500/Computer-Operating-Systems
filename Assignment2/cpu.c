#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"ram.h"
#include"interpreter.h"
#include"shell.h"

int isRunning = 0;//By default it is true

struct CPU{
    int IP; //Points to the next instruction to execute
    char IR[1000];  //Store the current instructions
    int quanta;  //qunata == 2
};

struct CPU cpu;//The only one CPU!


void setIP(int pc){
    cpu.IP = pc;
}

//Execute quanta number of instructions from IP
void run(int quanta){
    //set available to false when running
    isRunning = 1;
    
    int counter = quanta;

    while(counter > 0) {
        //Store the current instruction into IR
        strcpy(cpu.IR, getLineOfInstruction(cpu.IP));

        interpreter(cpu.IR);
        cpu.IP++;
        counter--;
    }
    //set available back to true when the run is finished.
    isRunning = 0;
}


