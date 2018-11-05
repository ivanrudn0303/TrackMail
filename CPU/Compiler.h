#ifndef COMPILER_H
#define COMPILER_H
#include <cstdint>
#include <vector>

#define ERROR_FILE 6

typedef struct Mark
{
	std::string Str;
	uint32_t Line;
} Mark;

#define COMMAND(name, num) int COMPILE_##name(const char*, const char*, char*, uint32_t*, std::vector<Mark>*);

#include "CommandList.h"
int COMPILE_RET(const char*, char*, uint32_t*);

char* ReadData(const char*);
char* BinaryCreate(const char*, uint32_t*);
int WriteData(const char*, uint32_t, const char*);

#undef COMMAND
#endif // !COMPILER_H
