#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "ram.h"
#include "kernel.h"

/*For example if the program has 4 lines or less of code, it returns 1. 
If greater than 4 and less that or equal to 8, it returns 2.*/
int countTotalPages(FILE *f) {
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
    double pageNums = ceil(count_lines / 4);
    int num = pageNums;
    return num;
}

/*FILE *f points to the beginning of the file in the backing store. 
The variable pageNumber is the desired page from the backing store.
The function loads the 4 lines of code from the page into the frame in ram[].*/
void loadPage(int pageNumber, FILE *f, int frameNumber) {
    int lineNum = pageNumber * 4;
    int count = 1;
    char chr = getc(f);
    //find the desired line
    while (count <= lineNum && chr != EOF) {
        if (chr == '\r' || chr == '\n') {
            count++;
        }
        chr = getc(f);
    }
    //load the 4 lines into ram
    char buf[100];
    while (fgets(buf,100, f) != NULL && count <= pageNumber+4){
        ram[frameNumber*4] = strdup(buf);
        count++;
    }
}

/*Use the FIFO technique to search ram[] for a frame (not equal to NULL). 
If one exists then return its index number, otherwise return -1.
*/
int findFrame(){
     for(int i = 0; i <10; i++){
        if(ram[i*4] == NULL){
            return i;
        }
    }
    return -1;
}

/*This function is only invoke when findFrame() returns a -1.
Use a random number generator to pick a random frame number. 
If the frame number does not belong to the pages of the active PCB 
(ie. it is not in its page table) 
then return that frame number as the victim, 
otherwise, starting from the randomly selected frame, iteratively increment the frame number 
(modulo-wise) until you come to a frame number not belong to the PCB’s pages, and return that number.*/
int findVictim(struct PCB* p){
    int randomIndex = rand() % 10;//Pick a random frame number in the range of (0-9)
    int exist;
    for(int j = 0; j < 10; j++){
        randomIndex = (randomIndex + j) % 10;//Increment one for each turn
        for(int i = 0; i < 10; i++){
            if(randomIndex == p->pageTable[i]){
                exist = 1;
            }
        }
        if(exist != 1){
            return randomIndex;
        }
    }
    return -1;//Problem occurs
}

/*The page tables must also be updated to reflect the changes.
If a victim was selected then the PCB page table of the victim must be updated.
We do this once for the PCB asking for the page fault, and we might do it again for the victim PCB (if there was one).
p->pageTable[pageNumber] = frameNumber (or = victimFrame).*/
int updatePageTable(struct PCB *p, int pageNumber, int frameNumber, int victimFrame) {
    if (findVictim(p) != -1) {
        p -> pageTable[pageNumber] = victimFrame;
    }
    else {
        p -> pageTable[pageNumber] = frameNumber;
    }
    return 0;
}

void pageFault() {

    struct ReadyQueue* headqueue = returnHeadPCB();
    struct ReadyQueue* tailqueue = returnTailPCB();

    char temp[100];
    sprintf(temp, "BackingStore/%d.txt", headqueue->pcb->pid);
    
    FILE* p = fopen(temp,"rt");
    
    //1. Determine the next page: PC_page++.
    headqueue -> pcb -> PC_page++;
    //2. If PC_page is beyond pages_max then the program terminates, 
    //otherwise we check to see if the frame for that page exists in the pageTable array.
    if (headqueue->pcb->PC_page > headqueue->pcb->pages_max) {
        headqueue->pcb = headqueue->next->pcb;
        tailqueue->pcb = tailqueue->next->pcb;
        return;
    }
    else {
        //3. If pageTable[PC_page] is valid then we have the frame number 
        //and can do PC=ram[frame] and reset PC_offset to zero.
        int framNum = findFrame();
        int victimFrame = findVictim(headqueue->pcb);
        if (framNum != -1) {
            loadPage(headqueue->pcb->PC_page, p, victimFrame);
            updatePageTable(headqueue->pcb,headqueue->pcb->PC_page,framNum, victimFrame);
        }
        else {
            loadPage(headqueue->pcb->PC_page, p, framNum);
            updatePageTable(headqueue->pcb,headqueue->pcb->PC_page,framNum,victimFrame);
        }
        
    }
}

int launcher(FILE *p){

    //1. Copy the entire file into the backing store.
    struct ReadyQueue* headqueue = returnHeadPCB();
    int Pid = headqueue->pcb->pid;
    char buf[50];
    snprintf(buf, sizeof(buf), "touch %d.txt",Pid);
    system("cd BackingStore/");
    system(buf);

    char fileName[50];
    snprintf(fileName, sizeof(fileName), "%d.txt",Pid);
    char ch;
    FILE *destination = fopen(fileName,"wt");;
    while((ch = fgetc(p)) != EOF) {
         fputc(ch, destination);
    }

    //2. Close the file pointer pointing to the original file.
    fclose(p);

    //loading two pages of the program into RAM when it is first launched.
    int currentPages = countTotalPages(destination);
    if (currentPages >= 2) {
        for (int i = 0; i < 2; i++) {
            int frameNum = findFrame();
            int victimNum = findVictim(headqueue->pcb);
            if (findFrame() != -1) {
                loadPage(headqueue->pcb->PC_page, destination, frameNum);
            }
            else {
                loadPage(headqueue->pcb->PC_page, destination, victimNum);
            }
            updatePageTable(headqueue->pcb, headqueue->pcb->PC_page, frameNum, victimNum);
        }
        currentPages = currentPages - 2;
        return 1;//success
    }
    else if (currentPages == 1){
        int frameNum = findFrame();
        int victimNum = findVictim(headqueue->pcb);
        if (findFrame() != -1) {
            loadPage(headqueue->pcb->PC_page, destination, findFrame());
        }
        else {
            loadPage(headqueue->pcb->PC_page, destination, findVictim(headqueue->pcb));
        }
        updatePageTable(headqueue->pcb, headqueue->pcb->PC_page, frameNum, victimNum);
        currentPages--;
        return 1;//success
    }
    else {
        //launched am empty file
        return 1;//success
    }
    return 0;//failed
}

