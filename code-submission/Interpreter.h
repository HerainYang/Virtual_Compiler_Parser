//
// Created by herain on 4/1/21.
//

#ifndef ASSIGNMENT2_INTERPRETER_H
#define ASSIGNMENT2_INTERPRETER_H

#endif //ASSIGNMENT2_INTERPRETER_H

#include "BasicOperation.h"

void interpretArgument(unsigned char type, unsigned char value, unsigned char *appearance, unsigned char *current);
void interpretInstruction(unsigned char opcodes);
void interpretProgramCode(unsigned char *programCode, int starting);
