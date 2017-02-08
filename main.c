// system file includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// header file includes
#include "dump.h"
#include "decodeStage.h"
#include "executeStage.h"
#include "fetchStage.h"
#include "forwarding.h"
#include "instructions.h"
#include "loader.h"
#include "memory.h"
#include "memoryStage.h"
#include "registers.h"
#include "status.h"
#include "tools.h"
#include "writebackStage.h"

// locally used function prototypes
void initialize(void);
void initializeForwarding(forwardType*);
void initializeStatus(statusType*);

//
// name     main
// purpose  loads .yo file into memory and simulates the
//          Y86 behavior
//
int main(int argc, char * args[])
{
    //create and initialize the necessary stucts
    initialize();

    //attempts to load the file to memory
    //  if unsuccessful, program is terminated
    if(!load(argc, args))
    {
        dumpMemory();
        exit(0);
    }

    //creates and initializes local variables
    unsigned int clockCount = 0;
    bool stop = false;

	//creates and initializes forwarding struct
    forwardType forward;
    initializeForwarding(&forward);

	//creates and initializes status struct
	statusType status;
	initializeStatus(&status);

    //calls all stages in reverse order to simluate pipelining
    //loops endlessly until there is an error or halt instruction
    while(!stop)
    {
        stop = writebackStage(&forward, &status);
        memoryStage(&forward, &status);
        executeStage(&forward, status);
        decodeStage(forward);
        fetchStage(forward);
        clockCount++;
    }

    //prints the total number of clock cycles completed
    printf("\nTotal clock cycles = %d\n", clockCount);
}

//
// name     initialize
// purpose  sets all struct values to defaults
//
void initialize(void)
{
    clearMemory();
    clearRegisters();
    clearFregister();
    clearDregister();
    clearEregister();
    clearMregister();
    clearWregister();
    initializeFuncPtrArray();
}

//
// name     initializeForwarding
// purpose  sets all forwarding values to defaults
//
void initializeForwarding(forwardType* forwarding)
{ 
    forwarding->e_dstE = RNONE;
    forwarding->e_valE = 0;
    forwarding->M_Cnd = 1;
    forwarding->M_icode = NOP;
    forwarding->M_valA = 0;
    forwarding->M_dstM = RNONE;
    forwarding->m_valM = 0;
    forwarding->M_dstE = RNONE;
    forwarding->M_valE = 0;
    forwarding->W_icode = NOP;
    forwarding->W_dstM = RNONE;
    forwarding->W_valM = 0;
	forwarding->W_dstE = RNONE;
	forwarding->W_valE = 0;
}

//
// name		initializeStatus
// purpose	sets all status values to defaults
//
void initializeStatus(statusType* status)
{
	status->m_stat = SAOK;
	status->W_stat = SAOK;
}

