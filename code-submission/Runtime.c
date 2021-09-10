//
// Created by herain on 4/6/21.
//

#include "Runtime.h"
int findMainFunction(struct programCodeStructure *pcs, unsigned char *memory, unsigned char *myRegister){
    myRegister[PROGRAMCOUNTER] = pcs->programCodeIndex[0];
    if(myRegister[PROGRAMCOUNTER] == 0xff){
        return FUNCENTRYNOTFOUND;
    }
    return SUCCESS;
}

void updatePC(unsigned char *myRegister, unsigned char opCode) {
    myRegister[PROGRAMCOUNTER] -= instructionSize(opCode);
}

unsigned char *getAddress(unsigned char type, unsigned char *value, unsigned char *memory, unsigned char *myRegister) {
    if (type == VALUE){
        return value;
    }
    if (type == REGISTER){
        return &myRegister[*value];
    }
    if (type == STACKSYMBOL){
        return &memory[myRegister[STACKFRAMEBOTTOM] + (*value)];
    }
    if (type == POINTER){
        return &memory[*getAddress(STACKSYMBOL, value, memory, myRegister)];
    }
    return SUCCESS;
}

/**
 * Copies the value at some point B in memory to another point A in memory (register or stack).
 * @param types
 * @param values
 * @param memory
 * @param myRegister
 */
void mov(unsigned char *types, unsigned char *values, unsigned char *memory, unsigned char *myRegister) {
    unsigned char valueB = *getAddress(types[1], &values[1], memory, myRegister);
    unsigned char *addressA = getAddress(types[0], &values[0], memory, myRegister);
    (*addressA) = valueB;
    //printf("content inside is %d\n", *(addressA));
}

/**
 * Calls another function the first argument is a single byte (using the VALUE type) containing the label of the calling function.
 * @param funcLabel
 * @param memory
 * @param myRegister
 */
int cal(unsigned char funcLabel, struct programCodeStructure *pcs, unsigned char *memory, unsigned char *myRegister) {
    if(runFunc(funcLabel, myRegister[PROGRAMCOUNTER], pcs, memory, myRegister) == STACKOVERFLOW){
        return STACKOVERFLOW;
    }
    return SUCCESS;
}

void ref(unsigned char *types, unsigned char *values, unsigned char *memory, unsigned char *myRegister){
    unsigned char valueB;
    if(types[1] == POINTER){
        //printf("there is a pointer, pointer contain %x\n", *getAddress(types[0], &values[1], memory, myRegister));
        valueB = *getAddress(STACKSYMBOL, &values[1], memory, myRegister);
    } else {
        valueB = myRegister[STACKFRAMEBOTTOM] + (values[1]);
    }
    unsigned char *addressA = getAddress(types[0], &values[0], memory, myRegister);
    //printf("%x now contain %x\n", values[0], valueB);
    (*addressA) = valueB;
}

void add(unsigned char *registerAddress, unsigned char *myRegister) {

    if(registerAddress[0] == PROGRAMCOUNTER)
    {
        myRegister[PCJUMP] = myRegister[registerAddress[1]];
        //printf("[ADD] set jumper as %d\n", myRegister[registerAddress[1]]);
        return;
    }
    myRegister[registerAddress[0]] += myRegister[registerAddress[1]];
    //printf("[ADD] after add, register has %d\n", myRegister[registerAddress[0]]);
}

void print(unsigned char type, unsigned char value, unsigned char *memory, unsigned char *myRegister){
    unsigned char outputValue = *getAddress(type, &value, memory, myRegister);
    printf("%d\n", outputValue);
}

void not(unsigned char registerAddress, unsigned char *myRegister) {
    myRegister[registerAddress] = ~myRegister[registerAddress];
}

void equ(unsigned char registerAddress, unsigned char *myRegister) {
    //printf("[EQU] testing\n");
    if (myRegister[registerAddress] == 0x00)
        myRegister[registerAddress] = 0x01;
    else
        myRegister[registerAddress] = 0x00;
}

int executeInstruction(unsigned char opCode, unsigned char *types, unsigned char *values, struct programCodeStructure *pcs, unsigned char *memory, unsigned char *myRegister) {
    switch (opCode) {
        case MOV:{
            mov(types, values, memory, myRegister);
            return SUCCESS;
        }
        case CAL:{
            if(cal(values[0], pcs, memory, myRegister) == STACKOVERFLOW)
                return STACKOVERFLOW;
            return SUCCESS;
        }
        case RET:{
            return RETURNSIG;
        }
        case REF:{
            ref(types, values, memory, myRegister);
            return SUCCESS;
        }
        case ADD:{
            add(values, myRegister);
            return SUCCESS;
        }
        case PRINT:{
            print(types[0], values[0], memory, myRegister);
            return SUCCESS;
        }
        case NOT:{
            not(values[0], myRegister);
            return SUCCESS;
        }
        case EQU:{
            equ(values[0], myRegister);
            return SUCCESS;
        }
    }
    return SUCCESS;
}

int runFunc(unsigned char funcLabel, unsigned char returnAddress, struct programCodeStructure *pcs, unsigned char *memory, unsigned char *myRegister) {
    if(myRegister[STACKTOP] == 0 && myRegister[STACKFRAMEBOTTOM] != 0) {
        return STACKOVERFLOW;
    }
    int offset = 8;
    if(pcs->programCodeIndex[funcLabel] == 0xff) {
        printf("function entry not found\n");
        return FUNCENTRYNOTFOUND;
    }
    myRegister[PROGRAMCOUNTER] = pcs->programCodeIndex[funcLabel];
    int PC = myRegister[PROGRAMCOUNTER];

    readNextLetter(&offset, 3, &PC, pcs->programCode);
    readNextLetter(&offset, 5, &PC, pcs->programCode);
    nextMemoryCell(&offset, &PC, 0xff);

    myRegister[PROGRAMCOUNTER] = PC;

    myRegister[STACKFRAMEBOTTOM] = myRegister[STACKTOP];
    myRegister[STACKTOP] += 32;

    while (1){
        PC = myRegister[PROGRAMCOUNTER];
        //printf("Enter a loop and pc is %d\n", PC);
        int cursor = PC;
        unsigned char instruction = readNextLetter(&offset, 3, &cursor, pcs->programCode);
        //printf("now pc is %d, executing %x\n", PC, instruction);
        if(myRegister[PCJUMP]!=0x00)
        {
            nextMemoryCell(&offset, &PC, instruction);
            myRegister[PROGRAMCOUNTER] = PC;
            myRegister[PCJUMP]--;
            //printf("[JUMP] don't execute this function: %x\n", instruction);
            continue;
        }
        unsigned char types[2];
        unsigned char values[2];
        int numOfArgument = getNumOfArgument(instruction);
        for(int j = 0; j < numOfArgument; j++){
            types[j] = readNextLetter(&offset, 2, &cursor, pcs->programCode);
            values[j] = readNextLetter(&offset, bitsToRead(types[j]), &cursor, pcs->programCode);
        }
        nextMemoryCell(&offset, &PC, instruction);
        myRegister[PROGRAMCOUNTER] = PC;
        int result = executeInstruction(instruction, types, values, pcs, memory, myRegister);
        if(PC < 0 && result != RETURNSIG){
            printf("no return\n");
            return STACKOVERFLOW;
        }
        PC = myRegister[PROGRAMCOUNTER];
        //printf("pc is %d\n", myRegister[PROGRAMCOUNTER]);
        //printf("\n");
        if(result == RETURNSIG)
        {
            break;
        } else if (result == STACKOVERFLOW){
            return STACKOVERFLOW;
        }
    }
    myRegister[PROGRAMCOUNTER] = returnAddress;
    myRegister[STACKTOP] -= 32;
    myRegister[STACKFRAMEBOTTOM] -= 32;
    if(myRegister[STACKFRAMEBOTTOM] < 0)
        myRegister[STACKFRAMEBOTTOM] = 0;
    return SUCCESS;
}
