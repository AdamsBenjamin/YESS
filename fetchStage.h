#ifndef FETCHSTAGE_H
#define FETCHSTAGE_H
#include "forwarding.h"
typedef struct 
{
    unsigned int predPC;
} fregister;

//prototypes for fetchStage utility functions
void fetchStage(forwardType);
fregister getFregister(void);
void clearFregister(void);
#endif
