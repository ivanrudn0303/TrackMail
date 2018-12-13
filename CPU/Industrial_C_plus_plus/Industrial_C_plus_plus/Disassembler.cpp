#include "Disassembler.h"
#include <algorithm>
#define EXIT_NUM 32

int FillMark(const char*, std::vector<uint32_t>*, uint32_t);
bool InVector(uint32_t, std::vector<uint32_t>*);

#define COMMAND(name, num) case num:\
	WRITE_##name(Code + Counter, &Counter, fd);\
	break;

int Disassemble(const char *Code, const char* path, uint32_t Size)
{
	std::vector<uint32_t> mark;
	int err = FillMark(Code, &mark, Size);
	if(err)
		return err;

	std::sort(mark.begin(), mark.end());
	uint32_t iterator = 0;
	uint32_t Counter = 0;

	FILE *fd = fopen(path, "w");
	if (fd == nullptr)
		return ERROR_FILE;

	while (Counter < Size)
	{
		if ((iterator < mark.size()) && (Counter == mark[iterator]))
		{
			fprintf(fd, "%lu: ", Counter);
			iterator++;
		}

		switch (Code[Counter])
		{
#include "CommandList.h"
		case EXIT_NUM:
			Counter++;
			fprintf(fd, "EXIT\n");
			break;
		default:
			return ERROR_COMMAND;
		}
	}
	fclose(fd);
	return 0;
}
#undef COMMAND

#define COMMAND(name, num) case num:\
	DISASSEMBLE_##name(Start + Counter, &Counter, Array);\
	break;

int FillMark(const char* Start, std::vector<uint32_t>* Array, uint32_t Size)
{
	uint32_t Counter = 0;
	while (Counter < Size)
	{
		switch (Start[Counter])
		{
#include "CommandList.h"
		case EXIT_NUM:
			Counter++;
			break;
		default:
			return ERROR_DATA;
		}
	}
	return 0;
}
#undef COMMAND

int DISASSEMBLE_PUSHR(const char* Code, uint32_t *PC, std::vector<uint32_t>*)
{
	(*PC) += 2;
	return 0;
}

int DISASSEMBLE_PUSH(const char* Code, uint32_t *PC, std::vector<uint32_t>*)
{
	(*PC) += 1 + sizeof(double);
	return 0;
}

int DISASSEMBLE_POP(const char* Code, uint32_t *PC, std::vector<uint32_t>*)
{
	(*PC) += 2;
	return 0;
}

int DISASSEMBLE_ADD(const char* Code, uint32_t *PC, std::vector<uint32_t>*)
{
	(*PC) += 1;
	return 0;
}

int DISASSEMBLE_SUB(const char* Code, uint32_t *PC, std::vector<uint32_t>*)
{
	(*PC) += 1;
	return 0;
}

int DISASSEMBLE_MUL(const char* Code, uint32_t *PC, std::vector<uint32_t>*)
{
	(*PC) += 1;
	return 0;
}

int DISASSEMBLE_DIV(const char* Code, uint32_t *PC, std::vector<uint32_t>*)
{
	(*PC) += 1;
	return 0;
}

int DISASSEMBLE_JL(const char* Code, uint32_t *PC, std::vector<uint32_t>*Array)
{
	return DISASSEMBLE_JMP(Code, PC, Array);
}

int DISASSEMBLE_JG(const char* Code, uint32_t *PC, std::vector<uint32_t>*Array)
{
	return DISASSEMBLE_JMP(Code, PC, Array);
}

int DISASSEMBLE_JE(const char* Code, uint32_t *PC, std::vector<uint32_t>*Array)
{
	return DISASSEMBLE_JMP(Code, PC, Array);
}

int DISASSEMBLE_JMP(const char* Code, uint32_t *PC, std::vector<uint32_t>*Array)
{
	(*PC) += 1;
	if (!InVector(*(uint32_t*)(Code + 1), Array))
		Array->push_back(*(uint32_t*)(Code + 1));
	(*PC) += sizeof(uint32_t);
	return 0;
}

int DISASSEMBLE_CALL(const char* Code, uint32_t *PC, std::vector<uint32_t>*Array)
{
	return DISASSEMBLE_JMP(Code, PC, Array);
}

int DISASSEMBLE_RET(const char* Code, uint32_t *PC, std::vector<uint32_t>*Array)
{
	(*PC) += 1;
	return 0;
}

int DISASSEMBLE_IN(const char* Code, uint32_t *PC, std::vector<uint32_t>*Array)
{
	(*PC) += 1;
	return 0;
}

int DISASSEMBLE_OUT(const char* Code, uint32_t *PC, std::vector<uint32_t>*Array)
{
	(*PC) += 1;
	return 0;
}


int WRITE_PUSHR(const char* Code, uint32_t *Counter, FILE* fd)
{
	fprintf(fd, "PUSHR r%d\n", (int)Code[1]);
	(*Counter) += 2;
	return 0;
}

int WRITE_PUSH(const char* Code, uint32_t *Counter, FILE* fd)
{
	fprintf(fd, "PUSH %lf\n", *(double*)(Code + 1));
	(*Counter) += 1 + sizeof(double);
	return 0;
}

int WRITE_POP(const char* Code, uint32_t *Counter, FILE* fd)
{
	fprintf(fd, "POP r%d\n", Code[1]);
	(*Counter) += 2;
	return 0;
}

int WRITE_ADD(const char* Code, uint32_t *Counter, FILE* fd)
{
	fprintf(fd, "ADD\n");
	(*Counter) += 1;
	return 0;
}

int WRITE_SUB(const char* Code, uint32_t *Counter, FILE* fd)
{
	fprintf(fd, "SUB\n");
	(*Counter) += 1;
	return 0;
}

int WRITE_MUL(const char* Code, uint32_t *Counter, FILE* fd)
{
	fprintf(fd, "MUL\n");
	(*Counter) += 1;
	return 0;
}

int WRITE_DIV(const char* Code, uint32_t *Counter, FILE* fd)
{
	fprintf(fd, "DIV\n");
	(*Counter) += 1;
	return 0;
}

int WRITE_JL(const char* Code, uint32_t *Counter, FILE* fd)
{
	fprintf(fd, "JL %lu\n", *(uint32_t*)(Code + 1));
	(*Counter) += 1 + sizeof(uint32_t);
	return 0;
}

int WRITE_JG(const char* Code, uint32_t *Counter, FILE* fd)
{
	fprintf(fd, "JG %lu\n", *(uint32_t*)(Code + 1));
	(*Counter) += 1 + sizeof(uint32_t);
	return 0;
}

int WRITE_JE(const char* Code, uint32_t *Counter, FILE* fd)
{
	fprintf(fd, "JE %lu\n", *(uint32_t*)(Code + 1));
	(*Counter) += 1 + sizeof(uint32_t);
	return 0;
}

int WRITE_JMP(const char* Code, uint32_t *Counter, FILE* fd)
{
	fprintf(fd, "JMP %lu\n", *(uint32_t*)(Code + 1));
	(*Counter) += 1 + sizeof(uint32_t);
	return 0;
}

int WRITE_CALL(const char* Code, uint32_t *Counter, FILE* fd)
{
	fprintf(fd, "CALL %lu\n", *(uint32_t*)(Code + 1));
	(*Counter) += 1 + sizeof(uint32_t);
	return 0;
}

int WRITE_RET(const char* Code, uint32_t *Counter, FILE* fd)
{
	fprintf(fd, "RET\n");
	(*Counter) += 1;
	return 0;
}

int WRITE_IN(const char* Code, uint32_t *Counter, FILE* fd)
{
	fprintf(fd, "IN\n");
	(*Counter) += 1;
	return 0;
}

int WRITE_OUT(const char* Code, uint32_t *Counter, FILE* fd)
{
	fprintf(fd, "OUT\n");
	(*Counter) += 1;
	return 0;
}

bool InVector(uint32_t value, std::vector<uint32_t>* Array)
{
	for (uint32_t i = 0; i < Array->size(); ++i)
		if ((*Array)[i] == value)
			return true;
	return false;
}