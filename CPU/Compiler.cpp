#include "Compiler.h"
#include "windows.h"
#include <cstdlib>
#include <cstdint>
#include <cstring>


char* FindEnd(const char*);
int AddMark(const char*, const char*, uint32_t, std::vector<Mark>*);
int ChangePosMark(const char*, uint32_t, std::vector<Mark>*);
int FillMark(char*, const std::vector<Mark>*);
int AddPaste(const char*, uint32_t, std::vector<Mark>*);
uint32_t FindMark(const char*, std::vector<Mark>*);

int Jumper(const char*, const char*, char*, uint32_t*, std::vector<Mark>*, char);

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

#define COMMAND(name, num) if((strstr(start, #name) != nullptr) && strstr(start, #name) < end)\
{\
	if(COMPILE_##name(start, end, code, &Pos, &MarkArray))\
	{\
		free(code);\
		return nullptr;\
	}\
}\
else 

char * BinaryCreate(const char *Data, uint32_t *FileSize)
{
	if (Data == nullptr)
		return nullptr;

	uint32_t Size = strlen(Data);
	uint32_t Pos = 0;
	const char *start = Data;
	const char *end = Data;
	char *code = (char*)malloc(4 * Size);
	std::vector<Mark> MarkArray;
	while (*end != '\0')
	{
		end = FindEnd(start);
		AddMark(start, end, Pos, &MarkArray);

#include "CommandList.h"
		if ((strstr(start, "EXIT") != nullptr) && strstr(start, "EXIT") < end)
		{
			if (COMPILE_EXIT(start, code, &Pos))
			{
				free(code);
				return nullptr;
			}
		}
		if (*end != '\0')
			start = end + 2;
	}
	FillMark(code, &MarkArray);
	*FileSize = Pos;
	if (Pos)
		return (char*)realloc(code, Pos);
	else
		return nullptr;
}
#undef COMMAND

char* FindEnd(const char* str)
{
	char *ret = min((char*)strchr(str, '\n'), (char*)strchr(str, '\r'));
	if (ret == nullptr)
		ret = (char*)strchr(str, '\0');
	char *comment = (char*)strchr(str, '/');
	if ((comment != nullptr) && (comment < ret))
		return comment;
	else
		return ret;
}

int AddMark(const char *Start, const char *End, uint32_t Pos, std::vector<Mark>* Array)
{
	const char* mark = strchr(Start, ':');
	if ((mark == nullptr) || (mark > End))
		return 0;
	if (FindMark(Start, Array) == ~0U(32))
		Array->push_back({ std::string(Start, mark), Pos , std::vector<uint32_t>() });
	else
		ChangePosMark(Start, Pos, Array);
	return 0;
}

int ChangePosMark(const char* Start, uint32_t Line, std::vector<Mark>* Array)
{
	for (uint32_t i = 0; i < Array->size(); ++i)
		if (strncmp((*Array)[i].Str.c_str(), Start, (*Array)[i].Str.size()) == 0)
		{
			(*Array)[i].Line = Line;
			break;
		}
	return 0;
}

int AddPaste(const char *Start, uint32_t Paste, std::vector<Mark> *Array)
{
	uint32_t Line = FindMark(Start, Array);
	if (Line == ~0U(32))
	{
		Array->push_back({ std::string(Start, FindEnd(Start) - Start), 0, std::vector<uint32_t>({ Paste }) });
		return 0;
	}

	for (uint32_t i = 0; i < Array->size(); ++i)
		if (strncmp((*Array)[i].Str.c_str(), Start, (*Array)[i].Str.size()) == 0)
		{
			(*Array)[i].Paste.push_back(Paste);
			break;
		}
	return 0;
}

int FillMark(char *Code, const std::vector<Mark>* Array)
{
	for(uint32_t i = 0; i < Array->size(); ++i)
		for (uint32_t j = 0; j < (*Array)[i].Paste.size(); ++j)
			*(uint32_t*)(Code + (*Array)[i].Paste[j]) = (*Array)[i].Line;
	return 0;
}

int COMPILE_EXIT(const char *Start, char *Code, uint32_t *Pos)
{
	Code[(*Pos)++] = BYTE_EXIT;
	return 0;
}

int COMPILE_PUSH(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>*)
{
	const char* num = strchr(Start, 'P');
	num = strchr(num, ' ');
	if ((num == nullptr) || (num > End))
		return 1;

	Code[(*Pos)++] = BYTE_PUSH;
	*(double*)(Code + *Pos) = atof(num);
	*Pos += sizeof(double);
	return 0;
}

int COMPILE_PUSHR(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>*)
{
	const char* num = strchr(Start, 'P');
	num = strchr(num, 'r');
	if ((num == nullptr) || (num > End))
		return 1;
	Code[(*Pos)++] = BYTE_PUSHR;
	Code[(*Pos)++] = num[1] - '0';
	return 0;
}

int COMPILE_POP(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>*)
{
	const char* num = strchr(Start, 'P');
	num = strchr(num, 'r');
	if ((num == nullptr) || (num > End))
		return 1;

	Code[(*Pos)++] = BYTE_POP;
	Code[(*Pos)++] = num[1] - '0';
	return 0;
}

int COMPILE_ADD(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>*)
{
	Code[(*Pos)++] = BYTE_ADD;
	return 0;
}

int COMPILE_SUB(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>*)
{
	Code[(*Pos)++] = BYTE_SUB;
	return 0;
}

int COMPILE_MUL(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>*)
{
	Code[(*Pos)++] = BYTE_MUL;
	return 0;
}

int COMPILE_DIV(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>*)
{
	Code[(*Pos)++] = BYTE_DIV;
	return 0;
}

int COMPILE_JMP(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>* Array)
{
	Code[(*Pos)++] = BYTE_JMP;
	return Jumper(Start, End, Code, Pos, Array, 'J');
}

int COMPILE_JE(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>* Array)
{
	Code[(*Pos)++] = BYTE_JE;
	return Jumper(Start, End, Code, Pos, Array, 'J');
}

int COMPILE_JL(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>* Array)
{
	Code[(*Pos)++] = BYTE_JL;
	return Jumper(Start, End, Code, Pos, Array, 'J');
}

int COMPILE_JG(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>* Array)
{
	Code[(*Pos)++] = BYTE_JG;
	return Jumper(Start, End, Code, Pos, Array, 'J');
}

int COMPILE_CALL(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>* Array)
{
	Code[(*Pos)++] = BYTE_CALL;
	return Jumper(Start, End, Code, Pos, Array, 'C');
}

int COMPILE_RET(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>* Array)
{
	Code[(*Pos)++] = BYTE_RET;
	return 0;
}

int COMPILE_IN(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>*)
{
	Code[(*Pos)++] = BYTE_IN;
	return 0;
}

int COMPILE_OUT(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>*)
{
	Code[(*Pos)++] = BYTE_OUT;
	return 0;
}

int Jumper(const char *Start, const char* End, char *Code, uint32_t *Pos, std::vector<Mark>* Array, char c)
{
	char alphabet[] = "abcdefghigklmnopqrstuvwxyz123456789";
	const char* num = strchr(Start, c);
	num = strchr(num, ' ');
	num = num + strcspn(num, alphabet);
	uint32_t Line = FindMark(num, Array);
//	if (Line != ~0U(32))
	AddPaste(num, *Pos, Array);
//	else
//		Array->push_back({ std::string(num, FindEnd(num) - num), 0, std::vector<uint32_t>({ *Pos }) });
	*(uint32_t*)(Code + *Pos) = Line;
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
