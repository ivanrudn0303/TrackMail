#ifndef PROCESSOR_H
#define PROCESSOR_H

#define ERROR_COMMAND 7
#define AMOUNT_REGISTERS 4

#define PROCESSOR_DEBUG

#ifdef PROCESSOR_DEBUG
#define STACK_DEBUG
#endif // PROCESSOR_DEBUG
#include "Stack.h"

typedef struct
{
	uint32_t PC;
	double r[AMOUNT_REGISTERS];
	Stack *St;
	Stack *RetSt;
} Processor;

#ifdef PROCESSOR_DEBUG
int ProcessorDump(const Processor*, const char*, int = 0);
#endif // PROCESSOR_DEBUG

int ProcessorCreate(Processor*);
int ProcessorExec(Processor*, const char*, uint32_t, double*, double*);
void ProcessorDestruct(Processor*);


#define COMMAND(func, num) int func(Processor*, const char*); 
#include "CommandList.h"
int EXIT(Processor*, double*);
#undef COMMAND

#endif // !PROCESSOR_H
