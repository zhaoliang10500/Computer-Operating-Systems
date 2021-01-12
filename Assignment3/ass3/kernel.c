#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"kernel.h"
#include"ram.h"
#include"cpu.h"
#include"shell.h"
#include"memorymanager.h"

int kernel(){ //shellUI

    char* str1 = "\nKernel 2.0 loaded!\n"
                "Welcome to the <Liang Zhao> shell!\n"
                "Shell version 3.0 Updated APRIL 2020\n";
    printf("%s",str1);
    lauchShell();
    return 0;      
}

void boot() {
    void initRam();
    system("mkdir BackingStore/");
}

int main() { 
    int error=0;
    boot(); // First : actions performed by boot 
    error = kernel(); // Second: actions performed by kernel 
    return error; 
}

int countNumOfInstructions(FILE *f) {
    double count_lines = 0;
    char chr;
 
   //extract character from file and store in chr
    chr = getc(f);
    while (chr != EOF) {
        //Count whenever new line is encountered
        if (chr == '\r' || chr == '\n') {
            count_lines = count_lines + 1;
        }
        //take next character from file.
        chr = getc(f);
    }
    return count_lines;
}

int myinit(char *fileName){
    
    FILE* p = fopen(fileName,"rt");
    int start = 0;
    int end = 0;
    //if there is no enough space in ram, return 3 means no enough space error.
    if (addToRAM(p, &start, &end) == 3){
        return 3;
    }
    int numOfInstructions = countTotalPages(p);
    //else
    //Make a new PCB
    struct PCB* newPCB = makePCB(start,end,countNumOfInstructions(p));
    //Add the PCB into the ready queue
    addToReady(newPCB);

    return 0;
    
}

struct ReadyQueue* head = NULL;
struct ReadyQueue* tail = NULL;

struct ReadyQueue* returnHeadPCB () {
    return head;
}

struct ReadyQueue* returnTailPCB () {
    return tail;
}

//Add the PCB to the tail of the Ready Queue
void addToReady(struct PCB* pcb){

    struct ReadyQueue* newProg = (struct ReadyQueue*)malloc(sizeof(struct ReadyQueue));
    newProg->pcb = pcb;

    //implement a ready queue FIFO and RR.

    //If no program in the ready queue
    if(head == NULL){
        head = newProg;
        newProg->next = newProg; // circular linkedlist to implement RR
        tail = newProg;
    }else{
        //Add the node to the end of the reay Queue
        newProg->next = head;
        tail->next = newProg;
        tail = newProg;
        tail->next = head;
    }
}


//main simulation executing loop
int scheduler(){

    if(RamNextAvailableIndex() >= 1000){
        return -1;
    }

    while(1){

        int quanta = 2;

        //Copies the PC from the PCB into the IP of the CPU
            
        //if ready queue is empty, run the program directly
        if(head == NULL){ 
            return 0;
        }
        //if not empty, 
        else{
            //current PC = head PC
            int currentPC = head -> pcb -> PC;
    
            //if all programs are completed, then we don't run any programs
            //we update the ready queue
            if(currentPC > head->pcb->end){
                //Remove PCB from the ready list
                struct ReadyQueue* tempNode = head;
                if(head != tail){//There are more than one programs in the queue
                    head = head -> next;
                    tail -> next = head;
                    free(tempNode);//Free the PCB
                }else{//There is only one program left in the queue, remove the program
                    free(tempNode);//Free this PCB
                    head = NULL;
                    tail = NULL;
                    return 0;
                }
                //clear RAM
                int start = head -> pcb -> start;
                int end = head -> pcb -> end;
                clearRAM(start,end);

            //if current PC is at the end, 
            //then for avoiding runing null instructions, we set quanta to 1
            //so that it will only run the last instruction.
            }else if(currentPC == head -> pcb -> end){

                quanta = 1;

                //set CPU IP to the current PC
                setIP(currentPC);
                run(quanta);

                //Update the PC after the quanta is finished
                head -> pcb -> PC = head -> pcb -> PC + quanta;
                  
                //remove the head(it is finished)
                tail = tail -> next;
                head = head -> next;
                    
        
            //If program is not completed, and there are more than two lines available.
            }else{
                setIP(currentPC);
                run(quanta);
                head -> pcb -> PC = (head -> pcb -> PC) + quanta;
                tail = tail -> next;
                head = head -> next;
            }
        }
        
    }
}