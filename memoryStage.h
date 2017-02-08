#ifndef MEMORYSTAGE_H
#define MEMORYSTAGE_H
#include "forwarding.h"
#include "status.h"

typedef struct
{
    unsigned int stat;
    unsigned int icode;
    unsigned int Cnd;
    unsigned int valE;
    unsigned int valA;
    unsigned int dstE;
    unsigned int dstM;
}mregister;

void memoryStage(forwardType*, statusType*);
void updateMregister(unsigned int, unsigned int, unsigned int,
                     unsigned int, unsigned int, unsigned int,
                     unsigned int);
mregister getMregister(void);
void clearMregister(void);
#endif
