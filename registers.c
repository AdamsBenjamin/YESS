// Simulates the register system for the Y86 simulator
// Also provides various methods for interacting with the registers
//
// authors  Benjamin Adams, Chun Zheng
// version  1.0

// header file includes
#include "registers.h"
#include "tools.h"

// char to hold condition codes
static unsigned char CC;
// array to simulate processor registers
static unsigned int reg[REGSIZE];

//
// name     getRegister
// purpose  retrieve data from the specified register
//
unsigned int getRegister(int regNum){
    return reg[regNum];
}

//
// name     setRegister
// purpose  store data into a specified register
//
// param    regNum      indicated the desired register
// param    regValue    the value to be stored in the register
//
void setRegister(int regNum, unsigned int regValue){
    reg[regNum] = regValue;
}

//
// name     clearRegisters
// purpose  flushes the contents of the registers and resets the CC
//
void clearRegisters(void){
    clearBuffer(((char *)(reg)), sizeof(reg));
    CC = 0x0;    
}

//
// name     setCC
// purpose  modify the current CC values
//
// param    bitNumber   location of the bit to be set
// param    value       value to which to set the bit
//
void setCC(unsigned int bitNumber, unsigned int value){
    CC &= ~(0x1 << bitNumber);
    CC |= (value << bitNumber);
}

//
// name     getCC
// purpose  check the current value of a bit in the CC
//
// param    bitNumber   location of the bit to be set
//
unsigned int getCC(unsigned int bitNumber){
    return (CC & (0x1 << bitNumber)) >> bitNumber;
}
