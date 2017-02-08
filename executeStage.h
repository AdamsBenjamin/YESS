#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H
#include "forwarding.h"
#include "status.h"

typedef struct
{
    unsigned int stat;
    unsigned int icode;
    unsigned int ifun;
    unsigned int valC;
    unsigned int valA;
    unsigned int valB;
    unsigned int dstE;
    unsigned int dstM;
    unsigned int srcA;
    unsigned int srcB;
}eregister;

void executeStage(forwardType*, statusType);
eregister getEregister(void);
void updateEregister(unsigned int stat, unsigned int icode, unsigned int ifun,
                     unsigned int valC, unsigned int valA, unsigned int valB,
                     unsigned int dstE, unsigned int dstM, unsigned int srcA, unsigned int srcB);
void initializeFuncPtrArray(void);
void clearEregister(void);
#endif
