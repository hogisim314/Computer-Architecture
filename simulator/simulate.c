/* LC-2K Instruction-level simulator */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8       /* number of machine registers */
#define MAXLINELENGTH 1000
typedef struct stateStruct
{
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;
int memoryValue;
int opcode;
int arg0, arg1, arg2;
int cnt;
void printState(stateType *);
int convertNum(int num);
void Parsing(stateType *statePtr, int *opcode, int *arg0, int *arg1, int *arg2);
int checkRegister(int num);
int offsetCheck(int offset);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2)
    {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL)
    {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }
    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
         state.numMemory++)
    {

        if (sscanf(line, "%d", state.mem + state.numMemory) != 1)
        {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    for (int i=0; i<8; i++) {
        state.reg[i]=0;
    }

    /* TODO: */
    while (1)
    {
        printState(&state);
        int flag = 0;
        memoryValue = state.mem[state.pc];
        opcode = (memoryValue >> 22) & 0b111;
        arg0 = (memoryValue >> 19) & 0b111;
        arg1 = (memoryValue >> 16) & 0b111;
        arg2 = (memoryValue)&0xFFFF;
        state.pc++;
        cnt++;
        if (!checkRegister(arg0) || !checkRegister(arg1))
        {
            exit(1);
        }
        if (opcode == 0b000)
        { // add
            if (!checkRegister(arg2))
                exit(1);
            state.reg[arg2] = state.reg[arg0] + state.reg[arg1];
        }
        else if (opcode == 0b001)
        { // nor
            if (!checkRegister(arg2))
                exit(1);
            state.reg[arg2] = ~(state.reg[arg0] | state.reg[arg1]);
        }
        else if (opcode == 0b010)
        { // lw
            int offset = convertNum(arg2);
            if (!offsetCheck(offset))
            {
                exit(1);
            }
            state.reg[arg1] = state.mem[state.reg[arg0] + offset];
        }
        else if (opcode == 0b011)
        { // sw
            int offset = convertNum(arg2);
            if (!offsetCheck(offset))
            {
                exit(1);
            }
            state.mem[state.reg[arg0] + offset] = state.reg[arg1];
        }
        else if (opcode == 0b100)
        { // beq
            int offset = convertNum(arg2);
            if (!offsetCheck(offset))
            {
                exit(1);
            }
            if (state.reg[arg0] == state.reg[arg1])
            {
                state.pc += offset;
            }
        }
        else if (opcode == 0b101)
        { // jalr
            state.reg[arg1] = state.pc;
            state.pc = state.reg[arg0];
        }
        else if (opcode == 0b110)
        { // halt
            flag = 1;
        }
        else if (opcode == 0b111)
        { // noop
        }
        else
        {
            exit(1);
        }

        if (flag)
        {
            break;
        }
    }
    printf("machine halted\n");
    printf("total of %d instructions executed\n", cnt);
    printf("final state of machine:\n");

    printState(&state);

    fclose(filePtr);
    exit(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++)
    {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++)
    {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
    /* convert a 16-bit number into a 32-bit Linux integer */
    if (num & (1 << 15))
    {
        num -= (1 << 16);
    }
    return (num);
}

void Parsing(stateType *statePtr, int *opcode, int *arg0, int *arg1, int *arg2)
{
    int memoryValue = statePtr->mem[statePtr->pc];

    *opcode = (memoryValue >> 22) & 0b111;
    *arg0 = (memoryValue >> 19) & 0b111;
    *arg1 = (memoryValue >> 16) & 0b111;
    *arg2 = (memoryValue)&0xFFFF;
}

int checkRegister(int num)
{
    if (num >= 0 && num <= 7)
    {
        return 1;
    }
    return 0;
}

int offsetCheck(int offset)
{
    if (offset < -32768 || offset > 32767)
        return 0; // error detected
    return 1;
}