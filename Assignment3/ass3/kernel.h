#include"pcb.h"

struct ReadyQueue{
    struct PCB* pcb;
    struct ReadyQueue* next;
};

int myinit(char *fileName);

void addToReady(struct PCB* pcb);

int scheduler();

int kernel();

struct ReadyQueue* returnHeadPCB();
struct ReadyQueue* returnTailPCB();