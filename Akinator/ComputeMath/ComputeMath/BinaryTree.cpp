#include "ErrorList.h"
#include "BinaryTree.h"
#include <cstdio>
#include <cstring>
#include <cmath>
#include <Windows.h>

int CreateAkinatorNode(Tree*, tdata* name, tdata* diff);
char* GetData(const char*);
char* FindStopArgument(char*);
const Tree* Find(const Tree*, const tdata*);
int NodeDump(const Tree* tr, FILE *f);

#ifdef DEBUG_BINARY_TREE
int Create(Tree *tr, Tree* top, char *str);
#else
int Create(Tree *tr, char *str);
#endif // DEBUG_BINARY_TREE


int Akinator(Tree* Tr)
{
	char c = 'e';
	std::cout << Tr->Data << "?\n";
	do
		std::cin >> c;
	while ((c != 'n') && (c != 'y'));
	if ((c == 'y') && (Tr->Right != nullptr))
		return Akinator(Tr->Right);
	else if ((c == 'n') && (Tr->Left != nullptr))
		return Akinator(Tr->Left);
	else if (c == 'n')
	{
		tdata name[256] = { 0 };
		tdata differ[256] = { 0 };
		std::cout << "What is it?\n";
		std::cin >> name;
		std::cout << "What the difference between " << name << " and " << Tr->Data << "\n";
		std::cin >> differ;
		tdata *BufName = (tdata*)malloc(sizeof(tdata) * (strlen(name) + 1));
		strcpy(BufName, name);
		tdata *BufDiff = (tdata*)malloc(sizeof(tdata) * (strlen(differ) + 1));
		strcpy(BufDiff, differ);
		return CreateAkinatorNode(Tr, BufName, BufDiff);
	}
	return 0;
}

int ObjectDescriptor(const Tree* tr, const tdata* obj)
{
	const Tree* fnd = Find(tr, obj);
	if(fnd == nullptr)
		return ERROR_NOT_EXIST;
	if (fnd == tr)
	{
		std::cout << tr->Data << std::endl;
	}
	else if (Find(tr->Left, obj) != nullptr)
	{
		std::cout << "not " << tr->Data << ", ";
		return ObjectDescriptor(tr->Left, obj);
	}
	else
	{
		std::cout << tr->Data << ", ";
		return ObjectDescriptor(tr->Right, obj);
	}
	return 0;
}

Tree* TreeCreate(int*res, const char *file)
{
	char* Str = GetData(file);
	if (Str == nullptr)
	{
		*res = ERROR_CREATING;
		return nullptr;
	}
	Tree* tr = (Tree*)calloc(1, sizeof(Tree));
#ifdef DEBUG_BINARY_TREE
	*res = Create(tr, nullptr, Str);
#else
	*res = Create(tr, Str);
#endif // DEBUG_BINARY_TREE
	free(Str);
	return tr;
}

int CreateAkinatorNode(Tree *tr, tdata * nam, tdata * diff)
{
	tr->Left = (Tree*)malloc(sizeof(Tree));
	tr->Left->Left = nullptr;
	tr->Left->Right = nullptr;
	tr->Left->Data = tr->Data;
	tr->Right = (Tree*)malloc(sizeof(Tree));
	tr->Right->Left = nullptr;
	tr->Right->Right = nullptr;
	tr->Right->Data = nam;
	tr->Data = diff;
	return 0;
}

char* GetData(const char* Name)
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
	char *data = (char*)malloc((size + 1) * sizeof(char));
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

char * FindStopArgument(char *str)
{
	int lftBracket = 0;
	int rhtBracket = 0;
	char* fnd = str;
	for (;; fnd++)
	{
		if (((*fnd == '#') || (*fnd == '\0') )&& (lftBracket == rhtBracket))
			break;
		else if (*fnd == '{')
			lftBracket++;
		else if (*fnd == '}')
			rhtBracket++;
	}
	if((fnd - 1 > str) && (*(fnd - 1) == '}'))
		*(fnd - 1) = '\0';
	return fnd;
}

const Tree * Find(const Tree *tr, const tdata *str)
{
	if (tr == nullptr)
		return nullptr;
	const Tree* fnd;
	if (!strcmp(str, tr->Data))
		return tr;
	else if (fnd = Find(tr->Left, str))
		return fnd;
	else
		return Find(tr->Right, str);
}

#define MINUSBRK(ptr) if(*ptr == '{')\
	ptr++;

#ifdef DEBUG_BINARY_TREE
int Create(Tree *tr, Tree* top, char *str)
{
	tr->Top = top;
#else
int Create(Tree *tr, char *str)
{
#endif // DEBUG_BINARY_TREE
	char* arg = strchr(str, '#');
	if (arg == nullptr)
	{
		tr->Data = (tdata*)calloc(strlen(str) + 1, sizeof(tdata));
		strcpy(tr->Data, str);
		tr->Left = nullptr;
		tr->Right = nullptr;
		return 0;
	}
	*arg = '\0';
	tr->Data = (tdata*)calloc(++arg - str, sizeof(tdata));
	strcpy(tr->Data, str);
	char *end = FindStopArgument(arg);
	MINUSBRK(arg);
	if (end > arg)
	{
		*end = '\0';
		tr->Left = (Tree*)calloc(1, sizeof(Tree));
#ifdef DEBUG_BINARY_TREE
		Create(tr->Left, tr, arg);
#else
		Create(tr->Left, arg);
#endif // DEBUG_BINARY_TREE
		arg = ++end;
	}
	else
	{
		arg = ++end;
		tr->Left = nullptr;
	}
	end = FindStopArgument(arg);
	MINUSBRK(arg);
	if (end > arg)
	{
		*end = '\0';
		tr->Right = (Tree*)calloc(1, sizeof(Tree));
#ifdef DEBUG_BINARY_TREE
		Create(tr->Right, tr, arg);
#else
		Create(tr->Left, arg);
#endif // DEBUG_BINARY_TREE
		arg = ++end;
	}
	else
	{
		arg = ++end;
		tr->Right = nullptr;
	}
	return 0;
}

#ifdef DEBUG_BINARY_TREE
int TreeError(const Tree* tr)
{
	if (tr == nullptr)
		return ERROR_POINTER;
	if ((tr->Top != nullptr) && ((tr->Top->Left != tr) && (tr->Top->Right != tr)))
		return ERROR_DATA;
	int res = 0;
	if (tr->Left)
		res = TreeError(tr->Left);
	if (res)
		return ERROR_DATA_LEFT;
	if (tr->Right)
		res = TreeError(tr->Right);
	if (res)
		return ERROR_DATA_RIGHT;
	return 0;
}	
#endif

int TreeDestruct(Tree* tr)
{
	if (tr == nullptr)
		return 0;
	free(tr->Data);
	TreeDestruct(tr->Right);
	TreeDestruct(tr->Left);
	free(tr);
	return 0;
}

int DotDump(const Tree* tr, FILE* f)
{
	if (!f)
		return ERROR_FILE;
	if (!tr)
		return ERROR_POINTER;
	int err = TreeError(tr);
	if (err)
		return err;
	fprintf(f, "digraph graphname {\n");
	NodeDump(tr, f);
	fprintf(f, "}\n");
	return 0;
}

int NodeDump(const Tree* tr, FILE *f)
{
	fprintf(f, "\t%lu [label=\"%s\"]\n", (uint32_t)tr, tr->Data);
	if (tr->Left)
	{
		NodeDump(tr->Left, f);
		fprintf(f, "\t%lu -> %lu\n", (uint32_t)tr, (uint32_t)(tr->Left));
	}
	if (tr->Right)
	{
		NodeDump(tr->Right, f);
		fprintf(f, "\t%lu -> %lu\n", (uint32_t)tr, (uint32_t)(tr->Right));
	}
	return 0;
}