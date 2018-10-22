#ifndef STACK_H
#define STACK_H
#include <cstdint>

#define ERROR_ALLOCATION 1
#define ERROR_DATA 2
#define ERROR_OVERFLOW 3
#define ERROR_UNDERFLOW 4
#define ERROR_POINTER 5


#define STACK_DEBUG


#ifdef STACK_DEBUG

//hash
uint64_t inline hash(uint64_t x)
{
	uint64_t a = 6364136223846793005;
	uint64_t c = 1442695040888963407;
	return (x * a + c);
}
//~hash

typedef struct Stack
{
	uint64_t Hash;
	uint32_t Size;
	double *Data;
	uint32_t Capacity;
} Stack;

uint64_t StackHash(const Stack*);
int StackError(const Stack*);

#else

typedef struct Stack
{
	uint32_t Size;
	double *Data;
	uint32_t Capacity;
} Stack;
#endif

int StackCreate(Stack*, uint32_t);
int Push(Stack*, double);
int Pop(Stack*, double*);
bool inline Empty(const Stack*);
int Clear(Stack*);
int Erase(Stack*);
#endif // !STACK_H

