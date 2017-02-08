// header file includes
#include "instructions.h"
#include "memory.h"
#include "memoryStage.h"
#include "registers.h"
#include "tools.h"
#include "writebackStage.h"

// M stage register
static mregister M;

// locally used function prototypes
void memControl(bool*, bool*);

//
// name     memoryStage
// purpose  simulate the memory stage of the pipelined Y86
//
// modifies W in writebackStage.c
//
void memoryStage(forwardType* forward, statusType* status)
{
    //creates local variables
    unsigned int m_valM = 0;
    unsigned int m_stat = SAOK;
    bool m_memRead = false;
    bool m_memWrite = false;
    bool m_memError = false;

    //send most forwarding variables
    forward->M_Cnd = M.Cnd;
    forward->M_icode = M.icode;
    forward->M_valA = M.valA;
    forward->M_valE = M.valE;
    forward->M_dstE = M.dstE;
    forward->M_dstM = M.dstM;

    memControl(&m_memRead, &m_memWrite);

    if(m_memWrite) putWord(M.valE, M.valA, &m_memError);
    else if(m_memRead)
    { 
        if(M.icode != POPL) m_valM = getWord(M.valE, &m_memError);
        else m_valM = getWord(M.valA, &m_memError);
    }

    m_stat = (m_memError)? SADR : M.stat;

    //send last forwarding variable
    //and update stat code
    forward->m_valM = m_valM;
    status->m_stat = m_stat;

    updateWregister(m_stat, M.icode, M.valE, m_valM, M.dstE, M.dstM);
}



////////////////////
//HELPER FUNCTIONS//
////////////////////

//
// name     memControl
// purpose  controls the memory read and write signals in
//          the memory stage
//
void memControl(bool* readSig, bool* writeSig)
{
    switch(M.icode)
    {
        case MRMOVL:
        case POPL:
        case RET:
            *readSig = true;
            break;
        case RMMOVL:
        case PUSHL:
        case CALL:
            *writeSig = true;
            break;
    }
}



////////////////////
//SYSTEM FUNCTIONS//
////////////////////

//
// name     updateMregister
// purpose  updates the values in the M register
//
// modifies M
// 
void updateMregister(unsigned int stat, unsigned int icode, unsigned int Cnd,
                     unsigned int valE, unsigned int valA, unsigned int dstE,
                     unsigned int dstM)
{
    M.stat = stat;
    M.icode = icode;
    M.Cnd = Cnd;
    M.valE = valE;
    M.valA = valA;
    M.dstE = dstE;
    M.dstM = dstM;
}

//
// name     getMregister
// purpose  returns a copy of the M register
//
mregister getMregister(void)
{
    return M;
}

//
// name     clearMregister
// purpose  reset the values in the M register to defaults
//
// modifies M
//
void clearMregister(void)
{
    M.stat = SAOK;
    M.icode = NOP;
    M.Cnd = 0;
    M.valE = 0;
    M.valA = 0;
    M.dstE = RNONE;
    M.dstM = RNONE;
}
