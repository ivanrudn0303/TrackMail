#ifndef COMPILER_H
#define COMPILER_H
#include <cstdint>
#include <vector>

#define ERROR_FILE 6

typedef struct Mark
{
	std::string Str;
	uint32_t Line;
	std::vector<uint32_t> Paste;
} Mark;

#define COMMAND(name, num) int COMPILE_##name(const char*, const char*, char*, uint32_t*, std::vector<Mark>*);

#include "CommandList.h"
int COMPILE_EXIT(const char*, char*, uint32_t*);
#undef COMMAND

char* ReadData(const char*);
char* BinaryCreate(const char*, uint32_t*);
int WriteData(const char*, uint32_t, const char*);


#define COMMAND(name, num) BYTE_##name = num,

enum Command
{
	#include "CommandList.h"
	BYTE_EXIT = 32
};
#undef COMMAND
#endif // !COMPILER_H