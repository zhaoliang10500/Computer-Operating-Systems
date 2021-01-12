struct PCB {
    int PC;
    int pageTable[10];//The index of array is the page number, assume that the program will have at most 40 lines of code
    int PC_page;//Which page the program is currently at
    int PC_offset;//Which offset the program is currently at
    int pages_max;//The total number of pages in the program
    
    int end, start;
    int pid;
    int numOfInstructions;
};


struct PCB* makePCB(int pageNum, int pid, int numOfInstructions);

