#include "Stack.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef STACK_DEBUG
#define STACK_CHECK(Stk) \
{\
	int err;\
	if(err = StackError(Stk))\
	{\
		StackDump(Stk, "logfile.txt");\
		return err; \
	}\
}\

#else
#define STACK_CHECK(Stk)

#endif // STACK_DEBUG


#ifdef STACK_DEBUG


/*
	malloc data = m (m * 72340172838076673)
	realloc data = r (r * 72340172838076673)
	clear data = c (c * 72340172838076673)
	pop data = p (p * 72340172838076673)
*/

int StackDump(const Stack *St, const char *path, int ErrorCode)
{
	if (St == nullptr)
		return ERROR_POINTER;

	FILE *fd;
	if (fopen_s(&fd, path, "a"))
		return ERROR_FILE;

	if(ErrorCode)
		fprintf(fd, "Dump Stack [0x%p], ERROR = %d, HASH = %llu\n", St, ErrorCode, St->Hash);
	else
		fprintf(fd, "Dump Stack [0x%p], ERROR = %d, HASH = %llu\n", St, StackError(St), St->Hash);

	fprintf(fd, "DATA [0x%p], SIZE = %d, CAPACITY = %d:\n", St->Data, St->Size, St->Capacity);
	for (uint32_t i = 0; i < St->Capacity; ++i)
	{
		uint64_t temp = *((uint64_t*)(St->Data + i));
		char c = ' ';
		if (temp == (72340172838076673 * 'c'))
			c = 'c';
		else if (temp == (72340172838076673 * 'm'))
			c = 'm';
		else if (temp == (72340172838076673 * 'r'))
			c = 'r';
		else if (temp == (72340172838076673 * 'p'))
			c = 'p';
		fprintf(fd, "[%d]\t(%c) %lg\n", i, c, (St->Data)[i]);
	}
	fprintf(fd, "\n");
	fclose(fd);
	return 0;
}

uint64_t StackHash(const Stack *St)
{
	uint64_t Sum = St->Size + St->Capacity;
	if (St->Data != nullptr)
		for (uint32_t i = 0; i < St->Size; ++i)
			Sum += *((uint64_t*)(St->Data + i));
	return hash(Sum);
}

int StackError(const Stack *St)
{
	if (St == nullptr)
	{
#ifdef STACK_DEBUG
		StackDump(St, "logfile.txt", ERROR_POINTER);
#endif // !STACK_DEBUG
		return ERROR_POINTER;
	}

	if ((St->Size > 0) && (St->Data == nullptr))
	{
#ifdef STACK_DEBUG
		StackDump(St, "logfile.txt", ERROR_POINTER);
#endif // !STACK_DEBUG
		return ERROR_POINTER;
	}

	uint64_t Sum = St->Size + St->Capacity;
	for (uint32_t i = 0; i < St->Size; ++i)
		Sum += *((uint64_t*)(St->Data + i));
	if (hash(Sum) == St->Hash)
		return 0;
	else
	{
#ifdef STACK_DEBUG
		StackDump(St, "logfile.txt", ERROR_DATA);
#endif // !STACK_DEBUG
		return ERROR_DATA;
	}
}

#endif // STACK_DEBUG

int Push(Stack *St, double value)
{
	STACK_CHECK(St);
	if (St->Capacity > St->Size)
	{
		(St->Data)[St->Size++] = value;

#ifdef STACK_DEBUG
		St->Hash = StackHash(St);
		STACK_CHECK(St);
#endif // STACK_DEBUG

		return 0;
	}
	else
	{
		if (St->Capacity)
			St->Capacity *= 2;
		else
			St->Capacity = 2;
		double *buf = (double*)realloc(St->Data, (St->Capacity) * sizeof(double));
		if (buf == nullptr)
		{
#ifdef STACK_DEBUG
			StackDump(St, "logfile.txt", ERROR_OVERFLOW);
#endif // !STACK_DEBUG
			return ERROR_OVERFLOW;
		}
		St->Data = buf;
		(St->Data)[St->Size++] = value;
#ifdef STACK_DEBUG
		for (uint32_t i = St->Size * sizeof(double); i < St->Capacity * sizeof(double); ++i)
			*((char*)(St->Data) + i) = 'r';
		St->Hash = StackHash(St);
		STACK_CHECK(St);
#endif // STACK_DEBUG
		return 0;
	}
}

int Pop(Stack *St, double *destination)
{
	STACK_CHECK(St);
	if (Empty(St))
	{
#ifdef STACK_DEBUG
		StackDump(St, "logfile.txt", ERROR_UNDERFLOW);
#endif // !STACK_DEBUG
		return ERROR_UNDERFLOW;
	}

	if (destination != nullptr)
		*destination = (St->Data)[--(St->Size)];
	else
		--(St->Size);

#ifdef STACK_DEBUG
		*((uint64_t*)(St->Data + St->Size)) = (72340172838076673 * 'p');
		St->Hash = StackHash(St);
		STACK_CHECK(St);
#endif // STACK_DEBUG
	return 0;
}

int StackCreate(Stack*St, uint32_t size)
{
	if (size)
	{
		St->Data = (double*)malloc(size * sizeof(double));
		if (St->Data == nullptr)
		{
#ifdef STACK_DEBUG
			StackDump(St, "logfile.txt", ERROR_ALLOCATION);
#endif // !STACK_DEBUG
			return ERROR_ALLOCATION;
		}

#ifdef STACK_DEBUG
		for (uint32_t i = 0; i < size * sizeof(double); ++i)
			*((char*)(St->Data) + i) = 'm';
#endif // STACK_DEBUG
	}
	else
		St->Data = nullptr;
	St->Capacity = size;
	St->Size = 0;

#ifdef STACK_DEBUG
	St->Hash = StackHash(St);
	STACK_CHECK(St);
#endif // STACK_DEBUG
	return 0;
}

bool inline Empty(const Stack *St)
{
	return (bool)(St->Size == 0);
}

int Clear(Stack *St)
{
	STACK_CHECK(St);

#ifdef STACK_DEBUG
	for (uint32_t i = 0; i < St->Size * sizeof(double); ++i)
		*((char*)(St->Data) + i) = 'c';
	St->Size = 0;
	St->Hash = StackHash(St);
	STACK_CHECK(St);

#else
	St->Size = 0;
#endif // STACK_DEBUG
	return 0;
}

int Erase(Stack *St)
{
	STACK_CHECK(St);

	if (Clear(St) != 0)
	{
#ifdef STACK_DEBUG
		StackDump(St, "logfile.txt", ERROR_DATA);
#endif // !STACK_DEBUG
		return ERROR_DATA;
	}

	free(St->Data);
	StackCreate(St, 0);
	
	STACK_CHECK(St);
	return 0;
}
