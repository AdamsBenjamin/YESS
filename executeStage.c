// header file includes
#include "executeStage.h"
#include "instructions.h"
#include "memoryStage.h"
#include "registers.h"
#include "tools.h"

// E stage register
static eregister E;
// function pointer array
static unsigned int (*funcPtr[16])(void);

// locally used function prototypes
unsigned int calcCnd(void);
unsigned int performCmovxx(void);
unsigned int performIrmovl(void);
unsigned int performRmmovl(void);
unsigned int performMrmovl(void);
unsigned int performOpl(void);
unsigned int performPush(void);
unsigned int performJxx(void);
unsigned int performCall(void);
unsigned int performRet(void);
unsigned int performPopl(void);
unsigned int performDumpl(void);
unsigned int retZero(void);
void updateCC(int);

//
// name     executeStage
// purpose  simulate the execute stage of the pipelined Y86
//
// modifies M in memoryStage.c
//
void executeStage(forwardType* forward, statusType status)
{
    //creates local variables
    unsigned int e_valE = 0;
    unsigned int e_cnd = 0;

    //simulates the ALU
    e_valE = (*funcPtr[E.icode])();

    //sets the CC, if applicable
    if((E.icode == OPL) && (status.m_stat == SAOK) && (status.W_stat == SAOK)) updateCC(e_valE);

    //obtains condition code
    e_cnd = calcCnd();

    //uses icode and e_cnd to change destination register
    //if necessary
    if((E.icode == CMOVXX) && !e_cnd) E.dstE = RNONE;

    //send forwarding values
    forward->e_dstE = E.dstE;
    forward->e_valE = e_valE;

    //updates the M register
    if((status.m_stat == SAOK) && (status.W_stat == SAOK))
    {
        updateMregister(E.stat, E.icode, e_cnd, e_valE, E.valA, E.dstE, E.dstM);
    }
    else
    {
        updateMregister(E.stat, NOP, 0, 0, 0, RNONE, RNONE);
    }
}



////////////////////
//HELPER FUNCTIONS//
////////////////////

//
// name     calcCnd
// purpose  generate the condition code
//          based on ifun and CC
//
unsigned int calcCnd(void)
{
    if(E.icode == CMOVXX)
    {
        switch(E.ifun)
        {
            case RRMOVL:
                return 1;
            case CMOVLE:
                return ((getCC(SF) ^ getCC(OF)) | getCC(ZF));
            case CMOVL:
                return (getCC(SF) ^ getCC(OF));
            case CMOVE:
                return getCC(ZF);
            case CMOVNE:
                return !getCC(ZF);
            case CMOVGE:
                return !(getCC(SF) ^ getCC(OF));
            case CMOVG:
                return (!(getCC(SF) ^ getCC(OF)) & !getCC(ZF));
            default:
                return 0;
        }
    }

    if(E.icode == JXX)
    {
        switch(E.ifun)
        {
            case JMP:
                return 1;
            case JLE:
                return ((getCC(SF) ^ getCC(OF)) | getCC(ZF));
            case JL:
                return (getCC(SF) ^ getCC(OF));
            case JE:
                return getCC(ZF);
            case JNE:
                return !getCC(ZF);
            case JGE:
                return !(getCC(SF) ^ getCC(OF));
            case JG:
                return (!(getCC(SF) ^ getCC(OF)) & !getCC(ZF));
            default:
                return 0;
        }
    }
    return 0;
}

//
// name     performCmovxx
// purpose  return value stored in E.valA
//
unsigned int performCmovxx(void)
{
    return E.valA;
}

//
// name     performIrmovl
// purpose  return immediate value to be loaded in dst register
//
unsigned int performIrmovl(void)
{
    return E.valC;
}

//
// name     performRmmovl
// purpose  return sum of E.valB and E.valC
//
unsigned int performRmmovl(void)
{
    return E.valB + E.valC;
}

//
// name     performMrmovl
// purpose  return sum of E.valB and E.valC
//
unsigned int performMrmovl(void)
{
    return E.valB + E.valC;
}

//
// name     performOpl
// purpose  perform basic operations on values in two source registers
//
// modifies CC
//
unsigned int performOpl(void)
{
    switch(E.ifun)
    {
        case ADDL:
            return  E.valB + E.valA;
        case SUBL:
            return E.valB - E.valA;
        case ANDL:
            return E.valB & E.valA;
        default:
            return E.valB ^ E.valA;
    }
}

// 
// name     performPush
// purpose  returns adjusted stack pointer value for after push
//
unsigned int performPushl(void)
{
    return E.valB - 4;
}

//
// name     performPop
// purpose  returns adjusted stack pointer value for after pop
//
unsigned int performPopl(void)
{
    return E.valB + 4;
}

//
// name     performJxx
// purpose  simulates the ALU during a JXX instruction
//
unsigned int performJxx(void)
{
    return 0;
}

//
// name     performCall
// purpose  simulates the ALU during a CALL instruction
//
unsigned int performCall(void)
{
    return E.valB - 4;
}

//
// name     performRet
// purpose  simulates the ALU during a RET instruction
//
unsigned int performRet(void)
{
    return E.valB + 4;
}

//
// name     performDump
// purpose  return the immediate value indicating information to dump 
//
unsigned int performDump(void)
{
    return E.valC;
}

//
// name     retZero
// purpose  returns 0
//
unsigned int retZero(void)
{
    return 0;
}

//
// name     updateCC
// purpose  to set the CC values produced by OPL operations
//
void updateCC(int valE)
{
    if(E.ifun == ADDL)
    { 
        if((0x80000000 & E.valA) == (0x80000000 & E.valB)
                && (0x80000000 & E.valA) != (0x80000000 & valE))
        {
            setCC(OF, 1);
        }
        else setCC(OF, 0);
    }
    else if(E.ifun == SUBL)
    {
        if((0x80000000 & E.valA) != (0x80000000 & E.valB)
                && (0x80000000 & E.valA) == (0x80000000 & valE))
        {
            setCC(OF, 1);
        }
        else setCC(OF, 0);
    }
    else setCC(OF, 0);

    if(valE == 0)
    {
         setCC(ZF, 1);
         setCC(SF, 0);
    }
    else
    {
        setCC(ZF, 0);
        if(valE < 0) setCC(SF, 1);
        else setCC(SF, 0);
    }
}



////////////////////
//SYSTEM FUNCTIONS//
////////////////////

//
// name     initializeFuncPtrArray
// purpose  sets the functions pointers in the funcPtr array
//
// modifies funcPtr
//
void initializeFuncPtrArray(void)
{   
    for(int i = 0; i < 16; i++)
    {
        funcPtr[i] = &retZero;
    }
    funcPtr[CMOVXX] = &performCmovxx;
    funcPtr[IRMOVL] = &performIrmovl;
    funcPtr[RMMOVL] = &performRmmovl;
    funcPtr[MRMOVL] = &performMrmovl;
    funcPtr[OPL] = &performOpl;
    funcPtr[PUSHL] = &performPushl;
    funcPtr[CALL] = &performCall;
    funcPtr[RET] = &performRet;
    funcPtr[JXX] = &performJxx; 
    funcPtr[POPL] = &performPopl;
    funcPtr[DUMP] = &performDump;
}

//
// name     updateEregiser
// purpose  updates the values in the E register
//
// modifies E    
//
void updateEregister(unsigned int stat, unsigned int icode, unsigned int ifun,
        unsigned int valC, unsigned int valA, unsigned valB, unsigned int dstE,
        unsigned int dstM, unsigned int srcA, unsigned int srcB)
{
    E.stat = stat;
    E.icode = icode;
    E.ifun = ifun;
    E.valC = valC;
    E.valA = valA;
    E.valB = valB;
    E.dstE = dstE;
    E.dstM = dstM;
    E.srcA = srcA;
    E.srcB = srcB;
}

//
// name     getEregister
// purpose  returns a copy of the E register
//
eregister getEregister(void)
{
    return E;
}

//
// name     clearEregister
// purpose  reset the values in the E register to defaults
//
// modifies E
//
void clearEregister(void)
{
    E.stat = SAOK;
    E.icode = NOP;
    E.ifun = 0;
    E.valC = 0;
    E.valA = 0;
    E.valB = 0;
    E.dstE = RNONE;
    E.dstM = RNONE;
    E.srcA = RNONE;
    E.srcB = RNONE;
}
