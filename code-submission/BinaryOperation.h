//
// Created by herain on 3/31/21.
//

#ifndef ASSIGNMENT2_BINARYOPERATION_H
#define ASSIGNMENT2_BINARYOPERATION_H

#endif //ASSIGNMENT2_BINARYOPERATION_H

#include "BasicOperation.h"
#include <stdio.h>
int bitsToRead(unsigned char type);
unsigned char readNextLetter_cp(int *offset, int getBit, int *currentByte, unsigned char *binaryCode);
int getNumOfArgument(unsigned char opcodes);
int readNextInstruction_cp(int *offset, int *currentByte, unsigned char *binaryCode, unsigned char *programCode, int *address);
int readNextFunction_cp(int *offset, int *currentByte, unsigned char *binaryCode, struct programCodeStructure *pcs, int *address, unsigned char *memory);
int readFile_cp(struct programCodeStructure *pcs, char *filePath, unsigned char *memory);
int checkArgument(unsigned char opcodes, unsigned char argumentType, int typeOrder);
void copyInstruction_cp(unsigned char *programCode, int *address, unsigned char content, int *paddingTo, int bitLength);
