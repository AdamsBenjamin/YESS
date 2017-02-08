#include <stdio.h>
// header file includes
#include "dump.h"
#include "instructions.h"
#include "memory.h"
#include "registers.h"
#include "tools.h"
#include "writebackStage.h"

// W stage register
static wregister W;

//
// name     writebackStage
// purpose  simulate the writeback stage of the pipelined Y86
//
bool writebackStage(forwardType* forward, statusType* status)
{
    //create local variables

    //checks status for continue
    if(W.stat !=  SAOK)
    {   
        //checks status for halt or error
        if(W.stat != SHLT)
        {   
            //prints message based on error status
            if(W.stat == SINS) printf("Invalid instruction\n");
            else printf("Invalid memory address\n");

            dumpProgramRegisters();
            dumpProcessorRegisters();
            dumpMemory();
        }
        return true;
    }

    //send forwarding values
    forward->W_icode = W.icode;
    forward->W_dstE = W.dstE;
    forward->W_valE = W.valE;
    forward->W_dstM = W.dstM;
    forward->W_valM = W.valM;

    //update status code
    status->W_stat = W.stat;

    //check for icode and perform according writes and outputs
    switch(W.icode)
    {   
        case HALT:
            return true;
        case CMOVXX:
            if(W.dstE != RNONE) setRegister(W.dstE, W.valE);
            break;
        case POPL:
            setRegister(W.dstE, W.valE);
        case MRMOVL:
            setRegister(W.dstM, W.valM);
            break;
        case RET:
        case CALL:
        case PUSHL:
        case IRMOVL:
        case OPL:
            setRegister(W.dstE, W.valE);
            break;
        case DUMP:
            if(W.valE & 0x1) dumpProgramRegisters();
            if(W.valE & 0x2) dumpProcessorRegisters();
            if(W.valE & 0x4) dumpMemory();
            break;
        default:
            break;
    }

    //if the program has reached this point,
    //it will continue for at least one more cycle
    return false;
}



////////////////////
//SYSTEM FUNCTIONS//
////////////////////

//
// name     updateWregister
// purpose  updates the values in the W register
//
// modifies W
//
void updateWregister(unsigned int stat, unsigned int icode, unsigned int valE,
                     unsigned int valM, unsigned int dstE, unsigned int dstM)
{
    W.stat = stat;
    W.icode = icode;
    W.valE = valE;
    W.valM = valM;
    W.dstE = dstE;
    W.dstM = dstM;
}

//
// name     getWregister
// purpose  returns a copy of the W register
//
wregister getWregister(void)
{
    return W;
}

//
// name     clearWregister
// purpose  reset the values in the W register to defaults
//
// modifies W
//
void clearWregister(void)
{
    W.stat = SAOK;
    W.icode = NOP;
    W.valE = 0;
    W.valM = 0;
    W.dstE = RNONE;
    W.dstM = RNONE;
}
