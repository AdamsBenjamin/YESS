// header file includes
#include "decodeStage.h"
#include "executeStage.h"
#include "registers.h"
#include "instructions.h"
#include "tools.h"

// D stage register
static dregister D;

// locally used function prototypes 
unsigned int selectFwdA(int, int, int, int,
                        int, int, int, int,
                        int, int, int, int);
unsigned int forwardB(int, int, int, int,
                      int, int, int, int,
                      int, int, int, int);
unsigned int getDstE(int);
unsigned int getDstM(int);
unsigned int getSrcA(int);
unsigned int getSrcB(int);

//
// name     decodeStage
// purpose  simulate the decode stage of the pipelined Y86
//
// modifies E in executeStage.c
//
void decodeStage(forwardType forward)
{
    //creates local variables
    unsigned int d_valA = 0;
    unsigned int d_valB = 0;
    unsigned int d_dstE = RNONE;
    unsigned int d_dstM = RNONE;
    unsigned int d_srcA = RNONE;
    unsigned int d_srcB = RNONE;
    unsigned int d_rvalA = 0;
    unsigned int d_rvalB = 0;

    //gets register numbers
    d_dstE = getDstE(D.icode);
    d_dstM = getDstM(D.icode);
    d_srcA = getSrcA(D.icode);
    d_srcB = getSrcB(D.icode);

    //gets register values
    d_rvalA = getRegister(d_srcA);
    d_rvalB = getRegister(d_srcB);

    //checks for register values in regards to
    //earlier operations and forwarded data
    d_valA = selectFwdA(d_srcA, d_rvalA, forward.e_dstE, forward.e_valE,
                        forward.M_dstM, forward.m_valM, forward.M_dstE, forward.M_valE,
                        forward.W_dstM, forward.W_valM, forward.W_dstE, forward.W_valE);
   
    d_valB = forwardB(d_srcB, d_rvalB, forward.e_dstE, forward.e_valE,
                      forward.M_dstM, forward.m_valM, forward.M_dstE, forward.M_valE,
                      forward.W_dstM, forward.W_valM, forward.W_dstE, forward.W_valE);

    //updates the E register
    updateEregister(D.stat, D.icode, D.ifun, D.valC, d_valA, d_valB, d_dstE, d_dstM, d_srcA, d_srcB); 
}



////////////////////
//HELPER FUNCTIONS//
////////////////////

//
// name     selectFwdA
// purpose  handle the forwarding logic in choosing d_valA
//          to be loaded into the execute stage register
//
unsigned int selectFwdA(int d_srcA, int d_rvalA, int e_dstE, int e_valE,
                        int M_dstM, int m_valM, int M_dstE, int M_valE,
                        int W_dstM, int W_valM, int W_dstE, int W_valE)
{
    if(D.icode == CALL || D.icode == JXX) return D.valP;
    if(d_srcA == RNONE) return 0;
    if(d_srcA == e_dstE) return e_valE;
    if(d_srcA == M_dstM) return m_valM;
    if(d_srcA == M_dstE) return M_valE;
    if(d_srcA == W_dstM) return W_valM;
    if(d_srcA == W_dstE) return W_valE;

    return d_rvalA;
}

//
// name     forwardB
// purpose  handle the forwarding logic in choosing d_valB
//          to be loaded into the execute stage register
//
unsigned int forwardB(int d_srcB, int d_rvalB, int e_dstE, int e_valE,
                      int M_dstM, int m_valM, int M_dstE, int M_valE,
                      int W_dstM, int W_valM, int W_dstE, int W_valE)
{
    if(d_srcB == RNONE) return 0;
    if(d_srcB == e_dstE) return e_valE;
    if(d_srcB == M_dstM) return m_valM;
    if(d_srcB == M_dstE) return M_valE;
    if(d_srcB == W_dstM) return W_valM;
    if(d_srcB == W_dstE) return W_valE;

    return d_rvalB;
}

// 
// name     getDstE
// purpose  uses icode to determine proper value for d_dstE
//
unsigned int getDstE(int icode)
{
    switch(icode)
    {
        case CMOVXX:
        case IRMOVL:
        case OPL:
            return D.rB;
            break;
        case PUSHL:
        case POPL:
        case CALL:
        case RET:
            return ESP;
            break;
        default:
            return RNONE;
            break;
    }
}

//
// name     getDstM
// purpose  uses icode to determine proper value for d_dstM
//
unsigned int getDstM(int icode)
{
    switch(icode)
    {
        case MRMOVL:
        case POPL:
            return D.rA;
            break;
        default:
            return RNONE;
            break;
    }
}

//
// name     getSrcA
// purpose  uses icode to determine proper value for d_srcA
//
unsigned int getSrcA(int icode)
{
    switch(icode)
    {
        case CMOVXX:
        case RMMOVL:
        case OPL:
        case PUSHL:
            return D.rA;
            break;
        case POPL:
        case RET:
            return ESP;
            break;
        default:
            return RNONE;
            break;
    }
}

//
// name     getSrcB
// purpose  uses icode to determine proper value for d_srcB
//
unsigned int getSrcB(int icode)
{
    switch(icode)
    {
        case OPL:
        case RMMOVL:
        case MRMOVL:
            return D.rB;
            break;
        case PUSHL:
        case POPL:
        case CALL:
        case RET:
            return ESP;
            break;
        default:
            return RNONE;
            break;
    }
}



////////////////////
//SYSTEM FUNCTIONS//
////////////////////

//
// name     updateDregister
// purpose  update the values held in the static dregister type struct D
//
// modifies D 
//
void updateDregister(unsigned int stat, unsigned int icode, unsigned int ifun,
                     unsigned int rA, unsigned int rB, unsigned int valC,
                     unsigned int valP)
{
    D.stat = stat;
    D.icode = icode;
    D.ifun = ifun;
    D.rA = rA;
    D.rB = rB;
    D.valC = valC;
    D.valP = valP;
}

//
// name     getDregister
// purpose  returns a copy of the D register
//
dregister getDregister(void)
{
    return D;
}

//
// name     clearDregister
// purpose  resets the values in the D register to defaults
//
// modifies D
void clearDregister(void)
{
    D.stat = SAOK;
    D.icode = NOP;
    D.ifun = 0;
    D.rA = RNONE;
    D.rB = RNONE;
    D.valC = RNONE;
    D.valP = RNONE;    
}
