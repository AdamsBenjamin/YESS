#include <stdio.h>
// header file includes
#include "decodeStage.h"
#include "fetchStage.h"
#include "forwarding.h"
#include "instructions.h"
#include "memory.h"
#include "registers.h"
#include "tools.h"

// F stage register
static fregister F;

// locally used function prototypes
unsigned int selectPC(forwardType);
bool instrValid(unsigned int);
bool needValC(unsigned int);
bool needRegIds(unsigned int);
unsigned int predictPC(unsigned int, unsigned int, unsigned int);


// 
// name     fetchStage
// purpose  simulates the fetch stage of the y86 processor
// 
// modifies F
// modifies D in decodeStage.c
//
void fetchStage(forwardType forward)
{
    //creates local variables
    unsigned char instruction = 0;
    unsigned int f_icode = 0;
    unsigned int f_ifun = 0;
    unsigned int f_rA = RNONE;
    unsigned int f_rB = RNONE;
    unsigned int f_valC = 0;
    unsigned int f_valP = 0;
	unsigned int f_stat = SAOK;
    bool f_memError = false;
	bool f_instrValid = true;

    //selects the next PC
    unsigned int f_pc = selectPC(forward);

    //fetchs instruction from memory and 
	//checks for valid icode
    instruction = getByte(f_pc, &f_memError);
	f_instrValid = instrValid(instruction >> 4);

    //sets f_icode and f_ifun according to memory access
	//and icode validity
    f_icode = (f_memError)? NOP : instruction >> 4;
    f_ifun = (f_memError)? 0x0 : instruction & 0xF;
   
    if(!f_instrValid || f_memError)
    {
        f_stat = (f_memError)? SADR : SINS;
    }
    
    //checks for halt instruction to set stat
    if(f_icode == HALT) f_stat = SHLT;
    f_pc++;

    //checks if the instruction needs register ids
    //  if so, the values are fetched
    if(needRegIds(f_icode))
    {
        unsigned int registers = getByte(f_pc, &f_memError);
        f_rA = registers >> 4;
        f_rB = registers & 0xF;
        f_pc += 1;
    }

    //checks if the instruction needs a valC
    //  if so, the value is fetched
    if(needValC(f_icode))
    {
        unsigned char b[4];
        for(int i = 0; i < 4; i++)
        {
            b[i] = getByte(f_pc + i, &f_memError);
        }
        f_valC = buildWord(b[0], b[1], b[2], b[3]);
        f_pc += 4;
    }

    //sets the valP
    f_valP = f_pc;

    //updates the D register
    updateDregister(f_stat, f_icode, f_ifun, f_rA, f_rB, f_valC, f_valP);

    //updates the F register
    F.predPC = predictPC(f_icode, f_pc, f_valP);
}



////////////////////
//HELPER FUNCTIONS//
////////////////////

//
// name     selectPC
// purpose  grab the value from the PC register
//
unsigned int selectPC(forwardType forward)
{
    if(forward.M_icode == JXX && !forward.M_Cnd) return forward.M_valA;
    if(forward.W_icode == RET) return forward.W_valM;
    return F.predPC;
}

//
// name     instrValid
// purpose  validate the icode
//
bool instrValid(unsigned int icode)
{
    return icode < 13;
}

//
// name     needValC
// purpose  check if the instruction needs a valC
//
bool needValC(unsigned int icode)
{
    switch(icode)
    {
        case IRMOVL:
        case RMMOVL:
        case MRMOVL:
        case JXX:
        case CALL:
        case DUMP:
            return true;
        default:
            return false;
    }
}

//
// name     needRegIds
// purpose  check if the instruction needs register ids
//
bool needRegIds(unsigned int icode)
{
    switch(icode)
    {
        case IRMOVL:
        case RMMOVL:
        case MRMOVL:
        case OPL:
        case CMOVXX:
        case PUSHL:
        case POPL:
            return true;
        default:
            return false;
    }
}

//
// name     predictPC
// purpose  update the pc
//
unsigned int predictPC(unsigned int icode, unsigned int f_pc, unsigned int valC)
{
    if(icode == JXX || icode == CALL) return valC;
    return f_pc;
}



////////////////////
//SYSTEM FUNCTIONS//
////////////////////

// 
// name     getFregister
// purpose  returns a copy of the F register
// 
fregister getFregister(void)
{
    return F;
}

// 
// name     clearFregister
// purpose  returns a copy of the F register
// 
// modifies F 
//
void clearFregister(void)
{
    F.predPC = 0;
}
