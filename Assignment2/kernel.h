#include"pcb.h"

struct ReadyQueue{
    struct PCB* pcb;
    struct ReadyQueue* next;
};

int myinit(char *fileName);

void addToReady(struct PCB* pcb);

int scheduler();