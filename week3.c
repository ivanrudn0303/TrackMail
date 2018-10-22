#include "Stack.h"
#include <cstdlib>

#ifndef STACK_DEBUG

int Push(Stack *St, double value)
{
	if (St->Capacity > St->Size)
	{
		(St->Data)[St->Size++] = value;
	}
	else
	{
		if(St->Capacity)
			St->Capacity *= 2;
		else
			St->Capacity = 2;
		double *buf = (double*)realloc(St->Data, (St->Capacity) * sizeof(double));
		if(buf == nullptr)
			return ERROR_OVERFLOW;
		St->Data = buf;
		(St->Data)[St->Size++] = value;
	}
	return 0;
}

int Pop(Stack *St, double *destination)
{
	if (!Empty(St))
	{
		*destination = St->Data[--(St->Size)];
		return 0;
	}
	else
		return ERROR_UNDERFLOW;
}

int StackCreate(Stack*St, uint32_t size)
{
	if (size)
	{
		(St->Data) = (double*)malloc(2 * size * sizeof(double));
		if (St->Data == nullptr)
			return ERROR_ALLOCATION;
	}
	else
		St->Data = nullptr;
	St->Capacity = 2 * size;
	St->Size = 0;
	return 0;
}

bool inline Empty(const Stack *St)
{
	return (bool)(St->Size == 0);
}

int Clear(Stack *St)
{
	St->Size = 0;
	return 0;
}

int Erase(Stack *St)
{
	Clear(St);
	free(St->Data);
	StackCreate(St, 0);
	return 0;
}
#else

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
	if (St != nullptr)
	{
		uint64_t Sum = St->Size + St->Capacity;
		if ((St->Size > 0) && (St->Data != nullptr))
			for (uint32_t i = 0; i < St->Size; ++i)
				Sum += *((uint64_t*)(St->Data + i));
		else if (St->Size > 0)
			return ERROR_POINTER;
		if (hash(Sum) == St->Hash)
			return 0;
		else
			return ERROR_DATA;
	}
	else
		return ERROR_POINTER;
}

int Push(Stack *St, double value)
{
	if (StackError(St))
		return ERROR_DATA;
	if (St->Capacity > St->Size)
	{
		(St->Data)[St->Size++] = value;
		St->Hash = StackHash(St);
		if (StackError(St))
			return ERROR_DATA;
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
			return ERROR_OVERFLOW;
		St->Data = buf;
		(St->Data)[St->Size++] = value;
		St->Hash = StackHash(St);
		if (StackError(St))
			return ERROR_DATA;
		return 0;
	}
}

int Pop(Stack *St, double *destination)
{
	if (StackError(St))
		return ERROR_DATA;
	if (!Empty(St))
	{
		*destination = (St->Data)[--(St->Size)];
		St->Hash = StackHash(St);
		if (StackError(St))
			return ERROR_DATA;
		return 0;
	}
	else
		return ERROR_UNDERFLOW;
}

int StackCreate(Stack*St, uint32_t size)
{
	if (size)
	{
		St->Data = (double*)malloc(2 * size * sizeof(double));
		if (St->Data == nullptr)
			return ERROR_ALLOCATION;
	}
	else
		St->Data = nullptr;
	St->Capacity = 2 * size;
	St->Size = 0;
	St->Hash = StackHash(St);
	if (StackError(St))
		return ERROR_DATA;
	return 0;
}

bool inline Empty(const Stack *St)
{
	return (bool)(St->Size == 0);
}

int Clear(Stack *St)
{
	if (StackError(St))
		return ERROR_DATA;
	St->Size = 0;
	St->Hash = StackHash(St);
	if (StackError(St))
		return ERROR_DATA;
	return 0;
}

int Erase(Stack *St)
{
	if (StackError(St))
		return ERROR_DATA;
	Clear(St);
	free(St->Data);
	StackCreate(St, 0);
	if (StackError(St))
		return ERROR_DATA;
	return 0;
}
#endif // !STACK_DEBUG
