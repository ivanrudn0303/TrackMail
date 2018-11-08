#include "Processor.h"
#include <cstdlib>
#include <iostream>

#define EXIT_NUM 32

#define COMMAND(func, num) \
case num:\
	if(func(Proc, Code))\
		return ERROR_COMMAND;\
	break;

#ifdef PROCESSOR_DEBUG

int ProcessorDump(const Processor *Proc, const char *path, int ErrorCode)
{
	if (Proc == nullptr)
		return ERROR_POINTER;

	FILE *fd = fopen(path, "a");
	if (fd == nullptr)
		return ERROR_FILE;

	fprintf(fd, "Dump Processor [0x%p], ERROR = %d\n", Proc, ErrorCode);
	fprintf(fd, "Program Counter: %lu\n", Proc->PC);
	fprintf(fd, "Registers: r[0] = %lg, r[1] = %lg, r[2] = %lg, r[3] = %lg\n\n", Proc->r[0], Proc->r[1], Proc->r[2], Proc->r[3]);
	
	fclose(fd);
	return StackDump(Proc->St, path);
}

#endif // PROCESSOR_DEBUG

int ProcessorCreate(Processor* Proc)
{
	Proc->St = (Stack*)malloc(sizeof(Stack));
	if (Proc->St == nullptr)
		return ERROR_ALLOCATION;

	Proc->RetSt = (Stack*)malloc(sizeof(Stack));
	if (Proc->RetSt == nullptr)
		return ERROR_ALLOCATION;
	Proc->PC = 0;
	return StackCreate(Proc->St, 0) || StackCreate(Proc->RetSt, 0);
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
#ifdef PROCESSOR_DEBUG
		ProcessorDump(Proc, "ProcessorLog.txt");
#endif // PROCESSOR_DEBUG
		switch (Code[Proc->PC])
		{
#include "CommandList.h"

		case EXIT_NUM:
			return EXIT(Proc, ret);
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
	Erase(Proc->RetSt);
	free(Proc->RetSt);
	Proc->PC = UINT32_MAX;
}

int PUSH(Processor *Pr, const char *code)
{
	int err = Push(Pr->St, *(double*)(code + Pr->PC + 1));
	Pr->PC += sizeof(double) + 1;
	return err;
}

int PUSHR(Processor *Pr, const char *code)
{
#ifdef PROCESSOR_DEBUG
	int err;
	if ((code[Pr->PC + 1] < AMOUNT_REGISTERS) && (code[Pr->PC + 1] >= 0))
		err = Push(Pr->St, *(Pr->r + code[Pr->PC + 1]));
	else
		return ERROR_COMMAND;
#else
	int err = Push(Pr->St, *(Pr->r + Pr->PC + 1));
#endif // PROCESSOR_DEBUG
	Pr->PC += 2;
	return err;
}

int POP(Processor *Pr, const char* code)
{
#ifdef PROCESSOR_DEBUG
	int err;
	if ((code[Pr->PC + 1] < AMOUNT_REGISTERS) && (code[Pr->PC + 1] >= 0))
		err = Pop(Pr->St, Pr->r + code[Pr->PC + 1]);
	else
		return ERROR_COMMAND;
#else
	int err = Pop(Pr->St, Pr->r + code[Pr->PC + 1]);
#endif // PROCESSOR_DEBUG
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

int JE(Processor *Pr, const char* code)
{
	double first, second;
	int err = Pop(Pr->St, &first);
	if (err)
		return err;
	err = Pop(Pr->St, &second);
	if (err)
		return err;
	if (first == second)
		Pr->PC = *(uint32_t*)(code + Pr->PC + 1);
	else
		Pr->PC += 1 + sizeof(uint32_t);
	return 0;
}

int CALL(Processor *Pr, const char* code)
{
	uint64_t RetAddr = Pr->PC + 1 + sizeof(uint32_t);
	int err = Push(Pr->RetSt, *(double*)(&RetAddr));
	if (err)
		return err;
	return JMP(Pr, code);
}

int RET(Processor *Pr, const char* code)
{
	if (Empty(Pr->RetSt))
		return ERROR_UNDERFLOW;
	uint64_t RetAddr;
	int err = Pop(Pr->RetSt, (double*)(&RetAddr));
	if (err)
		return err;
	Pr->PC = (uint32_t)RetAddr;
	return 0;
}

int EXIT(Processor *Pr, double* ret)
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
