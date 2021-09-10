//
// Created by herain on 4/6/21.
//

#include "BasicOperation.h"

int instructionSize(unsigned char opcodes){
    switch (opcodes) {
        case MOV:
        case REF:{
            return 3;
        }
        case CAL:
        case ADD:
        case PRINT:{
            return 2;
        }
        case RET:
        case NOT:
        case EQU:
        case 0xff:{ //special case
            return 1;
        }
    }
    return -1;
}

int bitsToRead(unsigned char type){
    switch (type) {
        case VALUE:{
            return 8;
        }
        case REGISTER:{
            return 3;
        }
        case STACKSYMBOL:
        case POINTER:{
            return 5;
        }
    }
    return -1;
}

int getNumOfArgument(unsigned char opcodes){
    if(opcodes == MOV || opcodes == REF || opcodes == ADD){
        return 2;
    } else if (opcodes == RET){
        return 0;
    } else {
        return 1;
    }
}

unsigned char readNextLetter(int *offset, int getBit, int *currentByte, unsigned char *programCode){
    unsigned char result = 0x00;
    int resultLength = getBit;
    if(*offset - getBit < 0){
        result = programCode[*currentByte] << (getBit - *offset);
        getBit = (getBit - *offset);
        *offset = 8;
        (*currentByte)--;
    }
    *offset = *offset - getBit;
    result = (result | (programCode[*currentByte] >> *offset));
    result = result & (BASIC >> (BYTE - resultLength));
    return result;
}



void nextMemoryCell(int *offset, int *currentByte, unsigned char opcodes) {
    *offset = 8;
    (*currentByte) -= instructionSize(opcodes);
}