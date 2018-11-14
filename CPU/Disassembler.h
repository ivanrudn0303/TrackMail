#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H
#include "Processor.h"
#include <vector>
#include <cstdint>
#include <iostream>


int Disassemble(const char*, const char*, uint32_t);

#define COMMAND(name, num) int DISASSEMBLE_##name(const char*, uint32_t*, std::vector<uint32_t>*);\
int WRITE_##name(const char*, uint32_t*, FILE*);
#include "CommandList.h"
#undef COMMAND

#endif
