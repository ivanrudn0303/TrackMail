#include "Processor.h"
#include <cstdlib>

#define COMMAND(func, num) \
case num:\
	if(func(Proc, Code))\
		return ERROR_COMMAND;\
	break;


int ProcessorCreate(Processor* Proc)
{
	Proc->St = (Stack*)malloc(sizeof(Stack));
	if (Proc->St == nullptr)
		return ERROR_ALLOCATION;
	Proc->PC = 0;
	return StackCreate(Proc->St, 0);
}

int ProcessorExec(Processor *Proc, const char *Code, uint32_t Argc, double* Argv, double* ret)
{
	if ((Argc && (Argv == nullptr)) || (Code == nullptr) || (Proc == nullptr))
		return ERROR_POINTER;
	int error;
	for (uint32_t i = 0; i < Argc; ++i)
	{
		error = Push(Proc->St, Argv[i]);
		if (error)
			return error;
	}
	while (true)
	{
		switch (Code[Proc->PC])
		{
		#include "CommandList.h"

		case 32:
			return RET(Proc, ret);
		default:
			return ERROR_COMMAND;
		}

	}

	return 0;
}


void ProcessorDestruct(Processor* Proc)
{
	Erase(Proc->St);
	free(Proc->St);
	Proc->PC = UINT32_MAX;
}

int PUSH(Processor *Pr, const char *code)
{
	int err = Push(Pr->St, *(double*)(code + Pr->PC + 1));
	Pr->PC += sizeof(double) + 1;
	return err;
}

int POP(Processor *Pr, const char* code)
{
	int err = Pop(Pr->St, Pr->r + code[Pr->PC + 1]);
	Pr->PC += 2;
	return err;
}

int ADD(Processor *Pr, const char* code)
{
	int err;
	double first, second;

	err = Pop(Pr->St, &first);
	if (err)
		return err;

	err = Pop(Pr->St, &second);
	if (err)
		return err;

	Pr->PC++;
	return Push(Pr->St, second + first);
}

int SUB(Processor *Pr, const char* code)
{
	int err;
	double first, second;

	err = Pop(Pr->St, &first);
	if (err)
		return err;

	err = Pop(Pr->St, &second);
	if (err)
		return err;

	Pr->PC++;
	return Push(Pr->St, first - second);
}

int MUL(Processor *Pr, const char* code)
{
	int err;
	double first, second;

	err = Pop(Pr->St, &first);
	if (err)
		return err;

	err = Pop(Pr->St, &second);
	if (err)
		return err;

	Pr->PC++;
	return Push(Pr->St, second * first);
}

int DIV(Processor *Pr, const char* code)
{
	int err;
	double first, second;

	err = Pop(Pr->St, &first);
	if (err)
		return err;

	err = Pop(Pr->St, &second);
	if (err)
		return err;

	Pr->PC++;
	return Push(Pr->St, first / second);
}

int JMP(Processor *Pr, const char* code)
{
	Pr->PC = *(uint32_t*)(code + Pr->PC + 1);
	return 0;
}

int RET(Processor *Pr, double* ret)
{
	if ((ret == nullptr) && (Pr->St->Size > 0))
		return ERROR_POINTER;
	int err;
	for (uint32_t i = 0; i < Pr->St->Size; ++i)
	{
		err = Pop(Pr->St, ret + i);
		if (err)
			return err;
	}
	return 0;
}
#undef COMMAND
