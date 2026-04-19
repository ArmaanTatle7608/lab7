/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
// Please finish the following functions for lab 8.
// Lab 8 will perform the following functions:
//   1. Fetch the code stored in memory
//   2. Decode the code and prepare for the execution of the code.
//   3. Setup the execution function for CPU.

// Lab 9 will perform the following functions:
//   4. Execute the code stored in the memory and print the results.
#include "header.h"
#include "lab8header.h"

extern char *regNameTab[N_REG];
extern int regFile[N_REG];
unsigned int PCRegister = 0;

void CPU(char *mem){
    unsigned int machineCode = 0;
    unsigned char opcode = 0;

    PCRegister = CODESECTION;

    do{
        printf("\nPC:%x\n", PCRegister);

        machineCode = CPU_fetchCode(mem, PCRegister);

        if (machineCode == 0)
            break;

        PCRegister += 4;

        opcode = CPU_Decode(machineCode);

        printf("Decoded Opcode is: %02X. \n", opcode);

        // CPU_Execution(opcode, machineCode, mem);
    }while (1);

    printRegisterFiles();
    printDataMemoryDump(mem);
}

// Lab 8 - Step 1
unsigned int CPU_fetchCode(char *mem, int codeOffset){
    unsigned int machineCode = 0;
    unsigned char byte0 = 0;
    unsigned char byte1 = 0;
    unsigned char byte2 = 0;
    unsigned char byte3 = 0;

    byte0 = (unsigned char)mem[codeOffset];
    byte1 = (unsigned char)mem[codeOffset + 1];
    byte2 = (unsigned char)mem[codeOffset + 2];
    byte3 = (unsigned char)mem[codeOffset + 3];

    // I rebuilt the 32-bit instruction using little-endian order to match how memory stores bytes
    machineCode = (unsigned int)byte0 |
                  ((unsigned int)byte1 << 8) |
                  ((unsigned int)byte2 << 16) |
                  ((unsigned int)byte3 << 24);

    if (DEBUG_CODE){
        printf("Fetched Machine Code: %08X\n", machineCode);
    }

    return machineCode;
}

// Lab 8 - Step 2
unsigned char CPU_Decode(unsigned int machineCode){
    unsigned char opcode = 0;
    unsigned char funct = 0;

    // I extract the opcode by shifting right 26 bits and masking the first 6 bits
    opcode = (unsigned char)((machineCode >> 26) & 0x3F);

    // If opcode is 0, Iw can treat it as an R-type instruction and return the function field instead
    if (opcode == 0){
        funct = (unsigned char)(machineCode & 0x3F);
        return funct;
    }

    return opcode;
}

// Lab 9 (leave as is)
void CPU_Execution(unsigned char opcode, unsigned int machineCode, char *mem){
    unsigned char rt = 0;
    switch (opcode)
    {
        case 0b101111:
            rt = (machineCode & 0x001F0000) >> 16;
            regFile[rt] = machineCode & 0x0000FFFF;
            PCRegister += 4;
            if (DEBUG_CODE){
                printf("Code Executed: %08X\n", machineCode);
                printf("****** PC Register is %08X ******\n", PCRegister);
            }
            break;

        case 0b100000:
            break;

        default:
            printf("Wrong instruction! You need to fix this instruction %02X %08X\n", opcode, machineCode);
            system("PAUSE");
            exit(3);
            break;
    }
}

// Lab 8 - Step 3
void printRegisterFiles(void){
    int i = 0;

    puts("\n---- Register File Dump ----");
    for (i = 0; i < N_REG; i++){
        printf("%-6s = 0x%08X\n", regNameTab[i], regFile[i]);
    }
}

// Lab 8 - Step 4
void printDataMemoryDump(char *mem){
    puts("\n---- Data Memory Dump ----");
    memory_dump(mem, DATASECTION, 256);
}
