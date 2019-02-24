#ifndef STACK_H
#define STACK_H
#include <cstdint>

#define ERROR_ALLOCATION 1
#define ERROR_DATA 2
#define ERROR_OVERFLOW 3
#define ERROR_UNDERFLOW 4
#define ERROR_POINTER 5
#define ERROR_FILE 6


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
#endif

class Stack
{
#ifdef STACK_DEBUG
	uint64_t Hash;
#endif
	uint32_t Size;
	double *Data;
	uint32_t Capacity;
public:
	Stack(uint32_t);
	~Stack();
	int Push(double);
	int Pop(double*);
	bool inline Empty() const;
	int Clear();
	int Erase();
#ifdef STACK_DEBUG
	uint64_t StackHash() const;
	int StackError() const;
	int StackDump(const char*, int = 0) const;
#endif // STACK_DEBUG
};
#endif // !STACK_H
