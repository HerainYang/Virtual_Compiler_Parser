//
// Created by herain on 4/6/21.
//

#ifndef ASSIGNMENT2_BASICOPERATION_H
#define ASSIGNMENT2_BASICOPERATION_H

#include <stdio.h>
#include <string.h>

#endif //ASSIGNMENT2_BASICOPERATION_H

#define BYTE 8
#define BASIC 0xFF
#define OFFSETFUNCENTRY 248
#define PROGRAMCOUNTER 0x07
#define STACKTOP 0x06
#define STACKFRAMEBOTTOM 0x05
#define PCJUMP 0x04

#define FUNCENTRYNOTFOUND -1
#define STACKOVERFLOW -2
#define FAILURE -1
#define SUCCESS 0
#define RETURNSIG 1

#define MEMORYSIZE 256
#define PROGRAMCODESIZE 512
#define REGISTERSIZE 8

#define VALUE 0x00
#define REGISTER 0x01
#define STACKSYMBOL 0x02
#define POINTER 0x03

#define MOV 0x00
#define CAL 0x01
#define RET 0x02
#define REF 0x03
#define ADD 0x04
#define PRINT 0x05
#define NOT 0x06
#define EQU 0x07

#ifndef PCS
#define PCS
struct programCodeStructure{
    unsigned char programCode[PROGRAMCODESIZE];
    unsigned char programCodeIndex[8];
};
#endif

int getNumOfArgument(unsigned char opcodes);
int bitsToRead(unsigned char type);
int instructionSize(unsigned char opcodes);
unsigned char readNextLetter(int *offset, int getBit, int *currentByte, unsigned char *programCode);
void nextMemoryCell(int *offset, int *currentByte, unsigned char opcodes);


