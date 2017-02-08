// Simulates a little endian memory system for the Y86 simulator
// Also provides various methods for interacting with the memory
//
// authors  Benjamin Adams, Chun Zheng
// version  1.2

#include <stdbool.h>
#include "memory.h"
#include "tools.h"

static unsigned int mem[MEMSIZE];

//
// name     fetch
// purpose  retrieve data from the simulated memory
//
// param    address     location from which to retrieve the data
// param    memError    pointer to bool type flag
//
unsigned int fetch(int address, bool* memError)
{
    // checks the address is within the memory bounds
    if(address < 0 || address >= MEMSIZE)
    {
        *memError = true;
        return 0;
    }
    return mem[address];
}

//
// name     store
// purpose  store data to the simulated memory
//
// param    address     location to which to store the data
// param    value       data to be stored in the memory
// param    memError    pointer to bool type flag
//
void store(int address, unsigned int value, bool* memError)
{
    // checks the address is within the memory bounds
    if(address < 0 || address >= MEMSIZE)
    {
        *memError = true;
        return;
    }
    mem[address] = value;
}

//
// name     getByte
// purpose  retrieve a specific byte from the simulated memory
//
// param    byteAddress     location of byte to be retrieved
// param    memError        pointer to bool type flag
//
unsigned char getByte(int byteAddress, bool* memError)
{
    // retrieves the word containing the byte to be returned
    unsigned int data = fetch(byteAddress >> 2, memError);
    // isolates and returns the specified byte
    return getByteNumber(byteAddress & 0x3, data);
}

//
// name     putByte
// purpose  store a single byte to the simulated memory
//
// param    byteAddress     location to which to store the data
// param    value           byte to be stored in memory
// param    memError        pointer to bool type flag
//
void putByte(int byteAddress, unsigned char value, bool* memError)
{
    // retrieves the word containing the byte to be altered
    unsigned int data = fetch(byteAddress >> 2, memError);
    // overwrites the byte in the specified location 
    data = putByteNumber((byteAddress & 0x3), value, data);
    // stores the modified word back to memory
    store(byteAddress >> 2, data, memError);
}

//
// name     getWord
// purpose  retrieve a chosen word from the simulated memory
//
// param    byteAddress     location of the word to be retrieved
// param    memError        pointer to bool type flag
//
unsigned int getWord(int byteAddress, bool* memError)
{
    // checks that the location provided falls on a word boundary
    if((byteAddress | ~0x3) == ~0x3)
    {
        unsigned int address = byteAddress;
        // retrieves and returns the word from memory
        return fetch(address >> 2, memError);
    }

    // sets flag that byteAddress was not the starting point of a word
    *memError = true;
    return 0;
}

//
// name     putWord
// purpose  store a word into memory
//
// param    byteAddress     location to which to store the word
// param    value           word to be stored in memory
// param    memError        pointer to bool type flag
//
void putWord(int byteAddress, unsigned int value, bool* memError)
{
    // checks that the location provided falls on a word boundary
    if((byteAddress | ~0x3) == ~0x3)
    {
        unsigned int address = byteAddress;
        //stored the word into memory
        store(address >> 2, value, memError);
        return;
    }

    // sets flag that byteAddress was not the starting point of a word
    *memError = true;
    return;
}

//
// name     clearMemory
// purpose  flush the contents of the simulated memory
//
void clearMemory(void)
{
    clearBuffer((char *)(mem), sizeof(mem));
}
