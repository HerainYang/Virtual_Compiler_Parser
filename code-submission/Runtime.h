//
// Created by herain on 4/6/21.
//

#ifndef ASSIGNMENT2_RUNTIME_H
#define ASSIGNMENT2_RUNTIME_H

#endif //ASSIGNMENT2_RUNTIME_H

#include "BasicOperation.h"

int findMainFunction(struct programCodeStructure *pcs, unsigned char *memory, unsigned char *myRegister);
int
runFunc(unsigned char funcLabel, unsigned char returnAddress, struct programCodeStructure *pcs, unsigned char *memory,
        unsigned char *myRegister);
void updatePC(unsigned char *myRegister, unsigned char opCode);
unsigned char *getAddress(unsigned char type, unsigned char *value, unsigned char *memory, unsigned char *myRegister);
int executeInstruction(unsigned char opCode, unsigned char *types, unsigned char *values, struct programCodeStructure *pcs, unsigned char *memory, unsigned char *myRegister);
void mov(unsigned char *types, unsigned char *values, unsigned char *memory, unsigned char *myRegister);
int cal(unsigned char funcLabel, struct programCodeStructure *pcs, unsigned char *memory, unsigned char *myRegister);
void ret(unsigned char *memory, unsigned char *myRegister);
void ref(unsigned char *types, unsigned char *values, unsigned char *memory, unsigned char *myRegister);
void add(unsigned char *registerAddress, unsigned char *myRegister);
void print(unsigned char type, unsigned char value, unsigned char *memory, unsigned char *myRegister);
void not(unsigned char registerAddress, unsigned char *myRegister);
void equ(unsigned char registerAddress, unsigned char *myRegister);
