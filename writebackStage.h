#ifndef WRITEBACKSTAGE_H
#define WRITEBACKSTAGE_H
#include <stdbool.h>
#include "forwarding.h"
#include "status.h"

typedef struct
{
    unsigned int stat;
    unsigned int icode;
    unsigned int valE;
    unsigned int valM;
    unsigned int dstE;
    unsigned int dstM;
} wregister;

bool writebackStage(forwardType*, statusType*);
void updateWregister(unsigned int, unsigned int, unsigned int,
                    unsigned int, unsigned int, unsigned int);

wregister getWregister(void);
void clearWregister(void);
#endif
