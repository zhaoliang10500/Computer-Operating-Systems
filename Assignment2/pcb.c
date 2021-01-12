#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"pcb.h"

//Create a PCB instance
struct PCB* makePCB(int start, int end){
    //Making a new PCB
    struct PCB* newPCB = (struct PCB*)malloc(sizeof(struct PCB));
    newPCB -> start = start;
    newPCB -> end = end;
    newPCB -> PC = start;//Initial position
    struct PCB* next;
    return newPCB;
}
