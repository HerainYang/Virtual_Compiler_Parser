//
// Created by herain on 3/31/21.
//
/*
 * In compile time, compiler will place program code in programCode starting at 0, ending position will be return by readFile_cp().
 */
#include "BinaryOperation.h"
unsigned char readNextLetter_cp(int* offset, int getBit, int* currentByte, unsigned char* binaryCode){
    if(*currentByte < 0){
        return 0xff;
    }
    unsigned char result = 0x00;
    if(*offset + getBit > BYTE){
        result = binaryCode[*currentByte] >> *offset;
        (*currentByte)--;
        if(*currentByte < 0){
            return 0xff;
        }
        getBit = (*offset + getBit) - BYTE;
        unsigned char bitsFromNextByte = binaryCode[*currentByte] & (BASIC >> (BYTE - getBit));
        result = result | (bitsFromNextByte << (BYTE - *offset));
        *offset = 0;
    } else {
        result = (binaryCode[*currentByte] >> *offset) & (BASIC >> (BYTE - getBit));
    }
    if(*offset+getBit >= BYTE){
        (*currentByte)--;
    }
    *offset = (*offset + getBit) % BYTE;
    return result;
}

void copyInstruction_cp(unsigned char *programCode, int *address, unsigned char content, int *paddingTo, int bitLength){
    if(*paddingTo - bitLength < 0){
        programCode[*address] = programCode[*address] | (content >> (bitLength - *paddingTo));
        bitLength = (bitLength - *paddingTo);
        *paddingTo = 8;
        (*address)--;
    }
    *paddingTo = *paddingTo - bitLength;
    programCode[*address] = programCode[*address] | (content << *paddingTo);
}

int checkArgument(unsigned char opcodes, unsigned char argumentType, int typeOrder) {
    switch (opcodes) {
        case 0x05:
            return 1;
        case 0x00:{
            //MOV
            if(typeOrder == 0){
                if(argumentType == VALUE){
                    printf("MOV error: %x\n", argumentType);
                    return -1;
                }
            }
            break;
        }
        case 0x01:{
            //CAL
            if(argumentType != 0x00){
                printf("CALL error: %x\n", argumentType);
                return -1;
            }
            break;
        }
        case 0x03:{
            //REF
            if(argumentType == 0x00){
                printf("REF error: %x\n", argumentType);
                return -1;
            }
            break;
        }
        case 0x04: //ADD
        case 0x06: //NOT
        case 0x07:{
            //EQU
            if(argumentType != 0x01)
                return -1;
            break;
        }
    }
    return 1;
}

int readNextInstruction_cp(int *offset, int *currentByte, unsigned char *binaryCode, unsigned char *programCode, int *address){
    int paddingTo = 8;

    unsigned char opcodes = readNextLetter_cp(offset, 3, currentByte, binaryCode);
    if(opcodes == 0xff && (*currentByte) < 0) {
        printf("File Format Error: Opcodes Incomplete\n");
        return FAILURE;
    }
    //printf("checking %x\n", opcodes);
    (*address) += instructionSize(opcodes);

    int cursor = (*address) - 1;
    copyInstruction_cp(programCode, &cursor, opcodes, &paddingTo, 3);
    unsigned char type;
    int num = getNumOfArgument(opcodes);
    for(int i = 0; i < num; i++){
        type = readNextLetter_cp(offset, 2, currentByte, binaryCode);
        if(type == 0xff) {
            printf("File Format Error: Type Incomplete\n");
            return FAILURE;
        }
        if(checkArgument(opcodes, type, i) == -1)
        {
            printf("Compile error at byte -%x: instruction has a invalid argument\n", *currentByte);
            return FAILURE;
        }
        copyInstruction_cp(programCode, &cursor, type, &paddingTo, 2);
        unsigned char argument = readNextLetter_cp(offset, bitsToRead(type), currentByte, binaryCode);
        if(argument == 0xff && (*currentByte) < 0) {
            printf("File Format Error: Argument Incomplete\n");
            return FAILURE;
        }
        copyInstruction_cp(programCode, &cursor, argument, &paddingTo, bitsToRead(type));
    }
    return 1;
}

int readNextFunction_cp(int *offset, int *currentByte, unsigned char *binaryCode, struct programCodeStructure *pcs, int *address, unsigned char *memory) {
    if(*currentByte < 0){
        return -1;
    }
    unsigned char numOfInstruction = readNextLetter_cp(offset, 5, currentByte, binaryCode);
    //printf("this func has %x function\n", numOfInstruction);
    for(int i = 0; i < numOfInstruction; i++){
        if(readNextInstruction_cp(offset, currentByte, binaryCode, pcs->programCode, address) == -1)
            return -1;
    }
    unsigned char funcLabel = readNextLetter_cp(offset, 3, currentByte, binaryCode);
    int paddingTo = 8;
    copyInstruction_cp(pcs->programCode, address, funcLabel, &paddingTo, 3);
    copyInstruction_cp(pcs->programCode, address, numOfInstruction, &paddingTo, 5);
    //printf("func name: %x and current byte is %d\n", funcLabel, *currentByte);
    pcs->programCodeIndex[funcLabel] = (*address);
    (*address)++;
    return 1;
}

int readFile_cp(struct programCodeStructure *pcs, char *filePath, unsigned char *memory) {
    unsigned char binaryCode[512];
    unsigned char c;
    FILE* input = fopen(filePath, "rb");
    if(input == NULL){
        printf("File not found\n");
        return -1;
    }
    int i = 0;
    while (fread(&c, 1, 1, input)){
        binaryCode[i] = c;
        //printf("%x\n",c);
        i++;
    }
    fclose(input);
    i--;
    int offset = 0;
    int address = 0;
    while(1){
        if(readNextFunction_cp(&offset, &i, binaryCode, pcs, &address, memory) == -1)
            return -1;
        if(i <= 0){
            break;
        }
    }
    return address;
}
