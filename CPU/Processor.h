#ifndef PROCESSOR_H
#define PROCESSOR_H

#define ERROR_COMMAND 7

#define COMMAND(func, name) int func(Processor*, const char*); 

//#define PROCESSOR_DEBUG

#ifdef PROCESSOR_DEBUG
#define STACK_DEBUG
#endif // PROCESSOR_DEBUG
#include "Stack.h"

typedef struct 
{
	uint32_t PC;
	double r[4];
	Stack *St;
} Processor;

int ProcessorCreate(Processor*);
int ProcessorExec(Processor*, const char*, uint32_t, double*, double*);
void ProcessorDestruct(Processor*);
#include "CommandList.h"

int RET(Processor*, double*);
#undef COMMAND
#endif // !PROCESSOR_H
