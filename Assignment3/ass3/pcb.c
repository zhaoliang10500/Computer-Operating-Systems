#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"pcb.h"

struct PCB* makePCB(int pageNum, int pid, int numOfInstructions){
    //Making a new PCB
    struct PCB* newPCB = (struct PCB*)malloc(sizeof(struct PCB));
    //Initiate the page table
    for(int i = 0; i < 10; i++){
        newPCB->pageTable[i] = -1;
    }
    //Used to track the copied files in the BackingStore
    newPCB->pid = pid;
    //current offset and current page are initialized at 0
    newPCB->PC_page = 0;
    newPCB->PC_offset = 0;

    //Set the total number of pages needed by the process
    //a constant
    newPCB->pages_max = pageNum;

    //Set num of instructions in the script
    newPCB->numOfInstructions = numOfInstructions;

    return newPCB;
}