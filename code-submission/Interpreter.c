//
// Created by herain on 4/1/21.
//

/*
 * A binary program can enter interpret state means it format is correct, but it might have argument error
 */
#include "Interpreter.h"

/*
 * MOV: 0b000, 0x00
 * CAL: 0b001, 0x01
 * RET: 0b010, 0x02
 * REF: 0b011, 0x03
 * ADD: 0b100, 0x04
 * PRINT: 0x101, 0x05
 * NOT: 0x110, 0x06
 * EQU: 0x111, 0x07
 */

char stkptrTransform(unsigned char value){
    if(value >= 0 && value <26)
        return (65+value);
    else
        return (97+value);
}

void interpretArgument(unsigned char type, unsigned char value, unsigned char *appearance, unsigned char *current) {
    if (type == VALUE){
        printf("VAL %d", value);
    } else if (type == REGISTER) {
        printf("REG %d", value);
    } else if (type == STACKSYMBOL) {
        if(appearance[value] == 0xff){
            appearance[value] = *current;
            (*current)++;
        }
        printf("STK %c", stkptrTransform(appearance[value]));
    } else {
        value = appearance[value];
        printf("PTR %c", stkptrTransform(value));
    }
}

void interpretInstruction(unsigned char opcodes){
    switch (opcodes) {
        case MOV:{
            printf("MOV");
            return;
        }
        case CAL:{
            printf("CAL");
            return;
        }
        case RET:{
            printf("RET");
            return;
        }
        case REF:{
            printf("REF");
            return;
        }
        case ADD:{
            printf("ADD");
            return;
        }
        case PRINT:{
            printf("PRINT");
            return;
        }
        case NOT:{
            printf("NOT");
            return;
        }
        case EQU:{
            printf("EQU");
            return;
        }
    }
}

void interpretProgramCode(unsigned char *programCode, int starting){
    int offset = 8;
    int currentByte = starting - 1;
    while (currentByte >= 0){
        unsigned char funcLabel = readNextLetter(&offset, 3, &currentByte, programCode);
        printf("FUNC LABEL %x\n", funcLabel);
        unsigned char numOfInstruction = readNextLetter(&offset, 5, &currentByte, programCode);
        nextMemoryCell(&offset, &currentByte, 0xff);
        unsigned char appearance[32];
        memset(appearance, 0xff, 32 * sizeof(unsigned char));
        unsigned char current = 0x00;
        for (int i = 0; i < numOfInstruction; i++){
            int cursor = currentByte;
            unsigned char instruction = readNextLetter(&offset, 3, &cursor, programCode);
            printf("    ");
            interpretInstruction(instruction);
            int numOfArgument = getNumOfArgument(instruction);
            if(numOfArgument != 0)
                printf(" ");
            for(int j = 0; j < numOfArgument; j++){
                unsigned char type = readNextLetter(&offset, 2, &cursor, programCode);
                interpretArgument(type, readNextLetter(&offset, bitsToRead(type), &cursor, programCode), appearance, &current);
                if(j != numOfArgument - 1){
                    printf(" ");
                }
            }
            nextMemoryCell(&offset, &currentByte, instruction);
            printf("\n");
        }
    }
}
