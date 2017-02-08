#ifndef DECODESTAGE_H
#define DECODESTAGE_H

#include "forwarding.h"

typedef struct
{
    unsigned int stat;
    unsigned int icode;
    unsigned int ifun;
    unsigned int rA;
    unsigned int rB;
    unsigned int valC;
    unsigned int valP;
} dregister;

void decodeStage(forwardType);
void updateDregister(unsigned int, unsigned int, unsigned int,
        unsigned int, unsigned int, unsigned int, unsigned int);

dregister getDregister(void);
void clearDregister(void);
#endif
