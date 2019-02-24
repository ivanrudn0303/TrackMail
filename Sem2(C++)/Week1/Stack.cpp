#include "Stack.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef STACK_DEBUG
#define STACK_CHECK(Stk) \
{\
	int err;\
	if(err = Stk->StackError())\
	{\
		Stk->StackDump("logfile.txt");\
		return err; \
	}\
}\

#else
#define STACK_CHECK(Stk)

#endif // STACK_DEBUG

Stack::Stack(uint32_t size)
{
	if (size)
	{
		this->Data = (double*)malloc(size * sizeof(double));
		if (this->Data == nullptr)
		{
#ifdef STACK_DEBUG
			StackDump("logfile.txt", ERROR_ALLOCATION);
#endif // !STACK_DEBUG
			return;
		}

#ifdef STACK_DEBUG
		for (uint32_t i = 0; i < size * sizeof(double); ++i)
			*((char*)(this->Data) + i) = 'm';
#endif // STACK_DEBUG
	}
	else
		this->Data = nullptr;
	this->Capacity = size;
	this->Size = 0;

#ifdef STACK_DEBUG
	this->Hash = StackHash();
#endif // STACK_DEBUG
	return;
}

Stack::~Stack()
{
	free(Data);
}

int Stack::Push(double value)
{
	STACK_CHECK(this);
	if (this->Capacity > this->Size)
	{
		(this->Data)[this->Size++] = value;

#ifdef STACK_DEBUG
		this->Hash = StackHash();
		STACK_CHECK(this);
#endif // STACK_DEBUG

		return 0;
	}
	else
	{
		if (this->Capacity)
			this->Capacity *= 2;
		else
			this->Capacity = 2;
		double *buf = (double*)realloc(this->Data, (this->Capacity) * sizeof(double));
		if (buf == nullptr)
		{
#ifdef STACK_DEBUG
			StackDump("logfile.txt", ERROR_OVERFLOW);
#endif // !STACK_DEBUG
			return ERROR_OVERFLOW;
		}
		this->Data = buf;
		(this->Data)[this->Size++] = value;
#ifdef STACK_DEBUG
		for (uint32_t i = this->Size * sizeof(double); i < this->Capacity * sizeof(double); ++i)
			*((char*)(this->Data) + i) = 'r';
		this->Hash = StackHash();
		STACK_CHECK(this);
#endif // STACK_DEBUG
		return 0;
	}
}

int Stack::Pop(double *destination)
{
	STACK_CHECK(this);
	if (Empty())
	{
#ifdef STACK_DEBUG
		StackDump("logfile.txt", ERROR_UNDERFLOW);
#endif // !STACK_DEBUG
		return ERROR_UNDERFLOW;
	}

	if (destination != nullptr)
		*destination = (this->Data)[--(this->Size)];
	else
		--(this->Size);

#ifdef STACK_DEBUG
	*((uint64_t*)(this->Data + this->Size)) = (72340172838076673 * 'p');
	this->Hash = StackHash();
	STACK_CHECK(this);
#endif // STACK_DEBUG
	return 0;
}

inline bool Stack::Empty() const
{
	return static_cast<bool>(this->Size == 0);
}

int Stack::Clear()
{
	STACK_CHECK(this);

#ifdef STACK_DEBUG
	for (uint32_t i = 0; i < this->Size * sizeof(double); ++i)
		*((char*)(this->Data) + i) = 'c';
	this->Size = 0;
	this->Hash = StackHash();
	STACK_CHECK(this);

#else
	St->Size = 0;
#endif // STACK_DEBUG
	return 0;
}

int Stack::Erase()
{
	STACK_CHECK(this);

	if (Clear() != 0)
	{
#ifdef STACK_DEBUG
		StackDump("logfile.txt", ERROR_DATA);
#endif // !STACK_DEBUG
		return ERROR_DATA;
	}

	free(this->Data);
	this->Data = nullptr;
	this->Capacity = 0;
	this->Size = 0;
#ifdef STACK_DEBUG
	this->Hash = StackHash();
#endif // !STACK_DEBUG
	STACK_CHECK(this);
	return 0;

}


#ifdef STACK_DEBUG


/*
	malloc data = m (m * 72340172838076673)
	realloc data = r (r * 72340172838076673)
	clear data = c (c * 72340172838076673)
	pop data = p (p * 72340172838076673)
*/


uint64_t Stack::StackHash() const
{
	uint64_t Sum = this->Size + this->Capacity;
	if (this->Data != nullptr)
		for (uint32_t i = 0; i < this->Size; ++i)
			Sum += *((uint64_t*)(this->Data + i));
	return hash(Sum);
}

int Stack::StackError() const
{
	if (this == nullptr)
	{
#ifdef STACK_DEBUG
		StackDump("logfile.txt", ERROR_POINTER);
#endif // !STACK_DEBUG
		return ERROR_POINTER;
	}

	if ((this->Size > 0) && (this->Data == nullptr))
	{
#ifdef STACK_DEBUG
		StackDump("logfile.txt", ERROR_POINTER);
#endif // !STACK_DEBUG
		return ERROR_POINTER;
	}

	uint64_t Sum = this->Size + this->Capacity;
	for (uint32_t i = 0; i < this->Size; ++i)
		Sum += *((uint64_t*)(this->Data + i));
	if (hash(Sum) == this->Hash)
		return 0;
	else
	{
#ifdef STACK_DEBUG
		StackDump("logfile.txt", ERROR_DATA);
#endif // !STACK_DEBUG
		return ERROR_DATA;
	}
}

int Stack::StackDump(const char *path, int ErrorCode) const
{
	if (this == nullptr)
		return ERROR_POINTER;

	FILE *fd;
	if (fopen_s(&fd, path, "a"))
		return ERROR_FILE;

	if (ErrorCode)
		fprintf(fd, "Dump Stack [0x%p], ERROR = %d, HASH = %llu\n", this, ErrorCode, this->Hash);
	else
		fprintf(fd, "Dump Stack [0x%p], ERROR = %d, HASH = %llu\n", this, StackError(), this->Hash);

	fprintf(fd, "DATA [0x%p], SIZE = %d, CAPACITY = %d:\n", this->Data, this->Size, this->Capacity);
	for (uint32_t i = 0; i < this->Capacity; ++i)
	{
		uint64_t temp = *((uint64_t*)(this->Data + i));
		char c = ' ';
		if (temp == (72340172838076673 * 'c'))
			c = 'c';
		else if (temp == (72340172838076673 * 'm'))
			c = 'm';
		else if (temp == (72340172838076673 * 'r'))
			c = 'r';
		else if (temp == (72340172838076673 * 'p'))
			c = 'p';
		fprintf(fd, "[%d]\t(%c) %lg\n", i, c, (this->Data)[i]);
	}
	fprintf(fd, "\n");
	fclose(fd);
	return 0;
}

#endif
