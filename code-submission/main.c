#include <stdio.h>
#include "BinaryOperation.h"
#include "Interpreter.h"
#include "Runtime.h"
#include <string.h>

int main(int argc, char* argv[]) {

    struct programCodeStructure pcs;
    memset(pcs.programCode, 0, sizeof(unsigned char) * PROGRAMCODESIZE);
    memset(pcs.programCodeIndex, 0xff, sizeof(unsigned char) * 8);
    unsigned char memory[MEMORYSIZE];
    memset(memory, 0xff, sizeof(unsigned char) * MEMORYSIZE);
    int length = readFile_cp(&pcs, argv[1], memory);
    if(length == -1)
        return 1;
#ifdef INTERPRET
    interpretProgramCode(pcs.programCode, length);
#endif
#ifdef RUNTIME
    unsigned char myRegister[REGISTERSIZE];
    myRegister[0x06] = 0;
    myRegister[STACKFRAMEBOTTOM] = 0x00;
    myRegister[PCJUMP] = 0x00;
    if(runFunc(0, myRegister[PROGRAMCOUNTER], &pcs, memory, myRegister) == STACKOVERFLOW)
    {
        printf("Stack overflow\n");
        return 1;
    }
#endif
    return 0;
}

