#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "loader.h"
#include "memory.h"
unsigned char grabDataByte(char *record, int index); bool load(int argc, char *argv[]); bool validFileName(char *record); bool discardRest(char *recentLine);
bool isAddress(char *address); bool isData(char *recentLine);
bool isSpaces(char *record, int index, int end);
bool checkHex(char *line, int index, int end); bool checkLine(char *recentLine);
int grabAddress(char *recentLine);
int countDataByte(char *recentLine);
int dataLength(char *record);

/*
 * Authors: Chun Zheng,Ben Adams *
 * Version: C
 *
 * Purpose: Loading the YESS file *          i have given about 1/50 of my life to this 
 *          Barry, please...let me pass this class.
 */
FILE *fp;
char str[80];
/*
 * Description: boolean load that determines if the file
 *              loaded correctly.
 *
 * @param: argc, *argv[] this takes the number of files and 
 *              takes the names of the files that are to be 
 *              loaded
 */
bool load(int argc, char *argv[])
{
    int prevAdd = 0;
    int address = -1;
    bool memError = false; 
    int prevSize= 0;
    int dataSize = 0;
    if(argc > 1 && validFileName(argv[1]))
    {
        char *filename = argv[1];
        fp = fopen(filename, "r");
        for(int count = 1; !feof(fp); count++) 
        {
            fgets(str, 80, fp);
            if(!(checkLine(str))) 
            {
                printf("Error on line %d\n", count);
                puts(str);
                fclose(fp);
                return false;
            }

            if(isAddress(str))
            {
                address = grabAddress(str);
                if(isData(str))
                {
                    dataSize = countDataByte(str);
                    if((!(isSpaces(str, 9 +(dataSize * 2), 21))) || (address != (prevAdd + prevSize) && !feof(fp)))
                    {
                        printf("Error on line %d\n",count);
                        puts(str);
                        fclose(fp);
                        return false;
                    }
                    for(int i = 0; i < countDataByte(str); i++)
                    {
                        // printf("line %d byte %x\n", count, grabDataByte(str, i));
                        putByte(address + i, grabDataByte(str, i), &memError);
                        if(memError)
                        {
                            printf("Error on line %d\n", count);
                            puts(str);
                            fclose(fp);
                            return false;
                        }
                    }
                    prevSize = dataSize;
                }
                else
                {
                    prevSize = 0;
                }
                if(prevAdd < address) prevAdd = address;
            }
            discardRest(str);
        }
        fclose(fp);
        return true;
    }
    printf("file opening failed\nusage: yess <filename>.yo\n");
    return false;
}


/*
 * Description: ValidFileName checks to see if the file
 *              name has a valid ".yo" extension at the 
 *              end of the input
 *
 * @param: *record, this takes in the fp line that contains the 
 *              file name  
 */ 
bool validFileName(char *record)
{
    if(record == NULL) {return false;}
    char *p;
    if((p = strrchr(record, '.')) != NULL)
    {
        if(strcmp(p, ".yo") == 0)
        {
            return true;
        }
    }
    return false;
}

/*
 * Description: discardRest takes in a *recentLine that is from the 
 *              parameter and it will discard anything that is over 
 *              the strlen or 80 chars long
 *
 * @param: *recentLine takes in the char of arrays recentLine from the 
 *              fileReader
 */
bool discardRest(char *recentLine)
{
    /*
     * checks for a new line and if it does read a newline then 
     * it returns true
     */
    for(int i = 0; i < strlen(recentLine); i++)
    {
        if(recentLine[i] == '\n') {
            return true;
        }
        /* if it doesnt read a newline 
         * then it returns false
         */
        if(feof(fp))
        {
            return false;
        }
    }
    char get = fgetc(fp);
    /*if it doesnt get to the end of file and is greater than 80 char
     *then it will discard the rest via the fgetc function. 
     *this is a garbage skipper. 
     */
    while(get != '\n' && get != EOF)
    {
        get = fgetc(fp);
    }
    return true;
}

/* 
 * Description: grab the address and check the address inside the 
 *              file each line at a time.  * * @param: *recentLine takes this from the fileReader or fgets
 */
int grabAddress(char *recentLine)
{
    if(isAddress(recentLine))
    {
        char address[4];
        int j = 4;
        for(int i = 0; i < 3; i++)
        {
            address[i] = recentLine[j];
            j++;
        }
        address[3] = 0;
        return (int)strtol(address, NULL, 16);
    }
    return 0;
}
/*
 * Description: This method takes the data line and counts the 
 *              bytes in the words.
 */
int countDataByte(char *recentLine)
{
    int count = 0;
    int i = 9;
    while(i < 21 && checkHex(recentLine, i, i + 1))
    {
        count++;
        i += 2;
    }
    return count;
}

/*
 * so i think i did this one wrong
 * because it does essentially what 
 * isData() would do, if isData() was
 * still a function.
 * think this wokrs. 
 */
bool isAddress(char *recentLine)
{
    bool status = true;
    if(recentLine[2] == '0' && recentLine[3] == 'x'
            && recentLine[7] == ':')
    {
        for(int i = 4; i < 7; i++)
        {
            if(!(isxdigit(recentLine[i])))
            {   
                status = false;
            }
        }
    }
    else
    {
        status = false;
    }
    return status;
}

/*
 * Description: isData takes the most recentLine and checks if the line
 *              is a valid hex digit as well as make sure that
 *              after the instruction set, that it has a pipe and 
 *              a valid instruction..i think
 *
 * @param: char *record, this seems pretty self explainatory at this point 
 *              but for consistency sake im gonna tell you that it takes 
 *              the most recentLine read and i bet you though i was gonna
 *              say bannana.
 */ 
bool isData(char *record)
{
    //calls on the dataLength helper method to find the end of the data
    // to then check for space after the Data, along with the pipe character
    //condition statments to affirm the data is all spaces or 
    //data and the rest are spaces.
    if(checkHex(record , 9, 10) && record[22] == '|') return true;
    return false;
}


/*
 * this takes the index and end which 
 * should be probably be i think 9 through 20
 * to check if its all spaces.
 * works 
 */
bool isSpaces(char *record, int start, int end)
{
    //this tracks where the error went awry
    //default true status that can be changed 
    bool status = true;
    /*
       this loops through the record from a index position
       and checks for spaces, as the method name indicates.
       god i love making comments, i could just do this for
       the rest of my life...its soooo fullfilling.
       */
    for(int start; start <= end; start++)
    {
        if(record[start] != ' ')
        {
            status = false;
        }
    }
    return status;
}

/*
 * Description: checkHex take in three parameters and checks for Hex
 *              using the isxdigit method.
 * @param: char *recentLine, int index, int end are all used to check 
 *          the hex at a particular instance in the line of instruction.
 */ 
bool checkHex(char *recentLine, int index, int end)
{
    for(int i = index; i <= end; i++)
    {
        //where check to make sure that its NOT a hex digit
        //so that we can return false
        if (!(isxdigit(recentLine[i])))
        {
            return false;
        }
    }
    return true;
}

/*
 * Description: grabDataByte takes the index and end of the *record line
 *              sent to it from the method call and finds the number and
 *              converts it to a binary representation of the byte
 *
 * @param: *record, *index, *end is given the start and end of a dataByte
 */
unsigned char grabDataByte(char *record, int index)
{
    char byte[3];
    byte[0] = record[9 + index * 2]; 
    byte[1] = record[(9 + index * 2) + 1];
    byte[2] = 0;
    return (char)strtol(byte, NULL, 16);
}

/*
 * Description: checkLine checks if the line is up to snuff
 *              by seeing if the address is actually an address
 *              and checks the length of the line and if everythings
 *              in the right place give or take a space lol.
 *
 * @param: *recentLine takes it from the fgets in the load bool 
 */
bool checkLine(char *recentLine)
{
    if(recentLine[0] == ' ' && recentLine[1] == ' '
            && recentLine[8] == ' ' && recentLine[22] == '|')
    {
        if(isAddress(recentLine) && isSpaces(recentLine, 9 , 21))
        {
            return true;
        }    
        else if(isSpaces(recentLine, 0, 21)) return true;
        else if(isAddress(recentLine)
                && isData(recentLine));
        else
        {
            return false;
        }
    }
    return false;
}

/*
 * Description: dataLength is a helper method that determines the
 *              length of the data so that another method can iterater
 *              up to that point when it reaches a ' '
 *
 * @param: *record takes the line given to it from the method all
 */
int dataLength(char *record)
{
    int count = 0; 
    for(int i = 9; record[i] != ' ' &&  i <= 21; i++)
    {
        count++;
    }
    return count;
}
