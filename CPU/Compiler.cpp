#include "Compiler.h"
#include "windows.h"
#include <cstdlib>
#include <cstdint>
#include <cstring>

#define COMMAND(name, num) if((strstr(start, #name) != nullptr) && strstr(start, #name) < end)\
{\
	if(COMPILE_##name(start, end, code, &Pos, &MarkArray))\
	{\
		free(code);\
		return nullptr;\
	}\
}\
else 

char* End(const char*);
uint32_t FindMark(const char*, std::vector<Mark>*);

char * ReadData(const char *Name)
{
	HANDLE hFile = CreateFile(Name, GENERIC_READ, FILE_SHARE_READ, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return nullptr;
	DWORD size = GetFileSize(hFile, nullptr), counter;
	if (size == INVALID_FILE_SIZE)
	{
		CloseHandle(hFile);
		return nullptr;
	}
	char *data = (char*)malloc(size + 1);
	ReadFile(hFile, data, size, &counter, nullptr);
	CloseHandle(hFile);
	if (size != counter)
	{
		free(data);
		return nullptr;
	}
	else
	{
		data[size] = '\0';
		return data;
	}
}

char * BinaryCreate(const char *Data, uint32_t *FileSize)
{
	if (Data == nullptr)
		return nullptr;

	uint32_t Size = strlen(Data);
	uint32_t Pos = 0;
	const char *start = Data;
	const char *end = Data, *mark;
	char *code = (char*)malloc(4 * Size);
	std::vector<Mark> MarkArray;
	while (*end != '\0')
	{
		end = End(start);
		mark = strchr(start, ':');
		if ((mark != nullptr) && (mark < end))
			MarkArray.push_back({std::string(start, mark), Pos});

		#include "CommandList.h"
		if ((strstr(start, "RET") != nullptr) && strstr(start, "RET") < end)
		{
			if (COMPILE_RET(start, code, &Pos))
			{
				free(code);
				return nullptr;
			}
		}
		if (*end != '\0')
			start = end + 1;
	}

	Pos = 0;
	start = Data;
	end = Data;
	while (*end != '\0')
	{
		end = End(start);

#include "CommandList.h"
		if ((strstr(start, "RET") != nullptr) && strstr(start, "RET") < end)
		{
			if (COMPILE_RET(start, code, &Pos))
			{
				free(code);
				return nullptr;
			}
		}
		if (*end != '\0')
			start = end + 1;
	}

	*FileSize = Pos;
	if (Pos)
		return (char*)realloc(code, Pos);
	else
		return nullptr;
}


char* End(const char* str)
{
	char *ret = (char*)strchr(str, '\n');
	if (ret == nullptr)
		ret = (char*)strchr(str, '\0');
	char *comment = (char*)strchr(str, '/');
	if ((comment != nullptr) && (comment < ret))
		return comment;
	else
		return ret;
}


int COMPILE_RET(const char *Start, char *Code, uint32_t *Pos)
{
	Code[(*Pos)++] = (char)32;
	return 0;
}

int COMPILE_PUSH(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>*)
{
	const char* num = strchr(Start, 'P');
	num = strchr(num, ' ');
	if ((num == nullptr) || (num > End))
		return 1;

	Code[(*Pos)++] = (char)4;
	*(double*)(Code + *Pos) = atof(num);
	*Pos += sizeof(double);
	return 0;
}

int COMPILE_POP(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>*)
{
	const char* num = strchr(Start, 'P');
	num = strchr(num, 'r');
	if ((num == nullptr) || (num > End))
		return 1;

	Code[(*Pos)++] = (char)8;
	Code[(*Pos)++] = num[1] - '0';
	return 0;
}

int COMPILE_ADD(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>*)
{
	Code[(*Pos)++] = (char)12;
	return 0;
}

int COMPILE_SUB(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>*)
{
	Code[(*Pos)++] = (char)16;
	return 0;
}

int COMPILE_MUL(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>*)
{
	Code[(*Pos)++] = (char)20;
	return 0;
}

int COMPILE_DIV(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>*)
{
	Code[(*Pos)++] = (char)24;
	return 0;
}

int COMPILE_JMP(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>* Array)
{
	char alphabet[] = "abcdefghigklmnopqrstuvwxyz123456789";
	Code[(*Pos)++] = (char)28;
	const char* num = strchr(Start, 'J');
	num = strchr(num, ' ');
	num = num + strcspn(num, alphabet);
	*(uint32_t*)(Code + *Pos) = FindMark(num, Array);
	*Pos += sizeof(uint32_t);
	return 0;
}

uint32_t FindMark(const char *Str, std::vector<Mark>* Array)
{
	for (uint32_t i = 0; i < Array->size(); ++i)
		if (strncmp((*Array)[i].Str.c_str(), Str, (*Array)[i].Str.size()) == 0)
			return (*Array)[i].Line;
	return ~0U(32);
}

int WriteData(const char *Data, uint32_t Size, const char* Path)
{
	HANDLE hFile = CreateFile(Path, GENERIC_WRITE, FILE_SHARE_READ, nullptr,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return ERROR_FILE;
	DWORD dwTemp = 0;
	WriteFile(hFile, Data, Size, &dwTemp, nullptr);
	if (Size != dwTemp)
	{
		CloseHandle(hFile);
		return ERROR_FILE;
	}
	CloseHandle(hFile);
	return 0;
}
#undef COMMAND
