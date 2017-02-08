#include <stdio.h>
#include <stdbool.h>
#include "tools.h"

/*
 * A library of simple bit manipulation tools.
 *
 * @name tools.c
 * @version 1/20/2015
 *
 * @author Benjamin Adams
 * @username adamsbt 
 */

/*
 * Gets specified bits from a given source.
 *
 * @params
 *      low - int argument specifying the lower
 *          end of the desired bits
 *      high - int argument specifying the higher
 *          end of the desired bits
 *      source - unsigned int argument from which
 *          the bits are taken
 *
 * @returns
 *      unsigned int containing the specified bits
 */
unsigned int getBits(int low, int high, unsigned int source)
{
    if (low >= 0 && low <= 31 
        && high >= 0 && high <= 31 
        && !(high < low))
    {
        source = source << (31 - high);
        source = source >> (low + (31 - high));
        return source;
    }
    return 0;
} 

/*
 * Sets each bit in the given range to 1.
 *
 * @params
 *      low - int argument specifying the lower
 *          end of the desired bits
 *      high - int argument specifying the higher
 *          end of the desired bits
 *      source - unsigned int argument from which
 *          the bits are taken
 *
 * @returns
 *       modified, unsigned int version of source argument
 */
unsigned int setBits(int low, int high, unsigned int source)
{
    if (low >= 0 && low <=31
        && high >=0 && high <= 31
        && !(high < low))
    {
        unsigned int mask = 0xFFFFFFFF << (31 - high);
        mask = mask >> (low + (31 - high));
        mask = mask << low;
        return source | mask;
    }
    return source;
}

/*
 * Sets each bit in the given range to 0.
 *
 * @params
 *      low - int argument specifying the lower
 *          end of the desired bits
 *      high - int argument specifying the higher
 *          end of the desired bits
 *      source - unsigned int argument from which
 *          the bits are taken
 *
 * @returns
 *      modified, unsigned int version of source argument
 */
unsigned int clearBits(int low, int high, unsigned int source)
{
    if (low >= 0 && low <= 31
        && high >= 0 && high <= 31
        && !(high < low))
    {
        unsigned int mask = 0xFFFFFFFF << (31 - high);
        mask = mask >> (low + (31 - high));
        mask = mask << low;
        return source & ~mask;
    }
    return source;
}

/*
 * Sets a given bit in a source to a specific value.
 *
 * @params
 *      bitNumber - int argument specifying the index
 *          of the bit to be altered
 *      bitValue - int argument specifying the desired
 *          value of the bit
 *      source - unsigned int argument that is to be
 *          modified
 *
 * @returns
 *      modified, unsigned int version of source argument
 */
unsigned int assignOneBit(int bitNumber, int bitValue, unsigned int source)
{
    if (bitNumber >= 0 && bitNumber <= 31)
    {
        unsigned int mask = 0xFFFFFFFF << (31 - bitNumber);
        mask = mask >> 31;
        mask = mask << bitNumber;
        if (bitValue == 0)  return source & ~mask;
        else if (bitValue == 1) return source | mask;
    }
        return source;
}

/*
 * Grabs the specified byte from the source.
 *
 * @params
 *      byteNo - int argument specifying the byte to 
 *          be returned
 *      source - unsigned int argument that contains
 *          the byte to be returned
 *
 * @returns
 *      unsigned char value of the specified byte
 */
unsigned char getByteNumber(int byteNo, unsigned int source)
{
    if (byteNo >= 0 && byteNo <=3)
    {
        source = source << ((3 - byteNo) * 8);
        source = source >> ((byteNo * 8) + (3 - byteNo) * 8);
        return source;
    }
    return 0;
}

/*
 * Replaces the specified byte in the source.
 *
 * @params
 *      byteNo - int argument specifying the byte to
 *          be replaced
 *      byteValue - unsigned char value specifying
 *          the value to be placed in the byte
 *      source - unsigned int argument which contains
 *          the byte to be changed
 *
 * @returns
 *      modified, unsigned int version of source argument 
 */
unsigned int putByteNumber(int byteNo, unsigned char byteValue, 
                           unsigned int source)
{
    if (byteNo >= 0 && byteNo <=3)
    {
        source = clearBits(byteNo * 8, (byteNo + 1) * 8 - 1, source);
        return source | (byteValue << byteNo * 8);
    }
    return source;
}

/*
 * Builds a word from four given bytes.
 *
 * @params
 *      byte3 - unsigned char argument containing the
 *          most significant byte
 *      byte2 - unsigned char argument containing the
 *          second most significant byte
 *      byte1 - unsigned char argument containing the
 *          third most significant byte
 *      byte0 - unsigned char argument containing the
 *          least significant byte
 *
 * @returns
 *      unsigned int version of the word constructed
 *          from the arguments
 */
unsigned int buildWord(unsigned char byte0, unsigned char byte1, 
                       unsigned char byte2, unsigned char byte3)
{
    unsigned int b3 = ((int) byte3) << 24;
    unsigned int b2 = ((int) byte2) << 16;
    unsigned int b1 = ((int) byte1) << 8;
    unsigned int b0 = (int) byte0;
    return b3 | b2 | b1 | b0;
}

/*
 * Checks an int argument for negativity.
 *
 * @params
 *      source - unsigned int argument to be checked
 *
 * @returns
 *      boolean value
 */
bool isNegative(unsigned int source)
{
    int s = source;
    if (s < 0) return 1;
    return 0;
}

/*
 * Expands a given value into its individual
 *      bits and represents it in a formatted
 *      fashion using ASCII characters.
 *      Spaces are positioned at every eighth
 *      index and the string array is null terminated.
 *
 * @params
 *      source - unsigned int argument to be 
 *          represented in binary
 *      bits - char array with a maximum index of 35
 *          that is used to store the ASCII characters
 *
 * @returns
 *      void
 */
void expandBits(unsigned int source, char bits[36])
{
    int i;
    for (i = 34; i >= 0 ; i--)
    {
        if (i == 8 || i == 17 || i == 26) bits[i] = ' ';
        else
        {
            bits[i] = source % 2 + 0x30;
            source = source >> 1;
        }
    }
    bits[35] = 0x0;
    return;
}

/*
 * Clears a given amount of bytes.
 *
 * @param
 *      buff - char pointer that gives the
 *          location of the first byte to be cleared
 *      size - int argument that specifies
 *          the numbter of bytes to be cleared
 *
 * @returns
 *      void
 */
void clearBuffer(char * buff, int size)
{
    int i;
    for (i = 0; i <= size; i++) buff[i] = 0;
}

