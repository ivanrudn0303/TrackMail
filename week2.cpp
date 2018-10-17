#include <iostream>
#include <windows.h>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cstdlib>

#define OPENING_ERROR 1
#define WRITING_ERROR 2

class Str
{
public:
	char *Start;
	uint32_t Size;
	bool operator >(const Str&) const;
	bool operator ==(const Str&) const;
	Str& operator =(const Str&);
	bool operator <(const Str&) const;
};

char* GetData(const char *);

template <typename T>
void MyQSort(T*, uint32_t, bool(*)(const T&, const T&));
template <typename T>
void MyQSort(T*, uint32_t);
int TextAdd(const Str*, uint32_t, const char*);

bool Comp(const Str&, const Str&);

//-----------------------------------------------------------------------------------

int main()
{
	setlocale(LC_ALL, "Rus");
	char* Text = GetData("C:/Users/user/Downloads/yevgen.txt");//GetData("C:/Users/user/Downloads/file.txt");
	if (Text == nullptr)
		return 1;
	uint32_t StrCounter = 0;
	char *interm = std::strchr(Text, '\n');

	for (char* search = Text; true; interm = std::strchr(search, '\n'))
	{
		if (interm != nullptr)
		{
			search = interm + 1;
			StrCounter++;
		}
		else
		{
			if (*search != '\0')
				StrCounter++;
			break;
		}
	}

	std::vector<Str> TextArray(StrCounter);
	StrCounter = 0;
	interm = std::strchr(Text, '\r');

	for (char* search = Text; true; interm = std::strchr(search, '\r'))
	{
		if (interm != nullptr)
		{
			TextArray[StrCounter] = { search, static_cast<uint32_t>(interm - search) };
			search = interm + 2;
			StrCounter++;
		}
		else
		{
			if (search != (interm = std::strchr(search, '\0')))
			{
				TextArray[StrCounter] = { search, static_cast<uint32_t>(interm - search) };
			}
			break;
		}
	}


	if (TextAdd(TextArray.data(), TextArray.size(), "C:/Users/user/Downloads/output.txt") != 0)
	{
		printf("Error with printing\n");
		return 1;
	}
	printf("First Part\n");
	MyQSort(TextArray.data(), TextArray.size());
	if (TextAdd(TextArray.data(), TextArray.size(), "C:/Users/user/Downloads/output.txt") != 0)
	{
		printf("Error with printing\n");
		return 1;
	}
	printf("Second Part\n");
	MyQSort(TextArray.data(), TextArray.size(), Comp);
	if (TextAdd(TextArray.data(), TextArray.size(), "C:/Users/user/Downloads/output.txt") != 0)
	{
		printf("Error with printing\n");
		return 1;
	}
	printf("Third Part\n");

	delete[] Text;
}
//----------------------------------------------------------------------------
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
	char *data = new char[size + 1];
	ReadFile(hFile, data, size, &counter, nullptr);
	CloseHandle(hFile);
	if (size != counter)
	{
		delete[] data;
		return nullptr;
	}
	else
	{
		data[size] = '\0';
		return data;
	}
}

bool Str::operator>(const Str &rht) const
{
	for(uint32_t i = 0; ; i++)
	{
		if (i == rht.Size && i < this->Size)
			return true;
		else if (i == this->Size && i < rht.Size)
			return false;
		else if (i == this->Size && i == rht.Size)
			return false;
		if (this->Start[i] != rht.Start[i])
			return this->Start[i] > rht.Start[i];
	}
}

bool Str::operator==(const Str &rht) const
{
	if (rht.Size == this->Size)
		return static_cast<bool>(!std::strcmp(this->Start, rht.Start));
	else
		return false;
}

Str & Str::operator=(const Str &rht)
{
	this->Size = rht.Size;
	this->Start = rht.Start;
	return *this;
}

bool Str::operator<(const Str &rht) const
{
	for(uint32_t i = 0; ; i++)
	{
		if (i == rht.Size && i < this->Size)
			return false;
		else if (i == this->Size && i < rht.Size)
			return true;
		else if (i == this->Size && i == rht.Size)
			return false;
		if (this->Start[i] != rht.Start[i])
			return this->Start[i] < rht.Start[i];
	}
}


template<typename T>
void MyQSort(T *Data, uint32_t Size, bool(*Comparator)(const T &, const T &))
{
	if (Size > 1)
	{
		uint32_t Pos = Size / 2;
		uint32_t Counter = 0;
		for (uint32_t i = 0; i < Size; i++)
			if (Comparator(Data[Pos], Data[i]))
				Counter++;

		std::swap(Data[Pos], Data[Counter]);
		Pos = 0;

		for (uint32_t i = 0; i < Size; i++)
			if (Comparator(Data[Counter], Data[i]))
			{
				if (Pos != i)
					std::swap(Data[Pos], Data[i]);
				Pos++;
			}
		MyQSort(Data, Counter, Comparator);
		MyQSort(Data + Counter + 1, Size - Counter - 1, Comparator);
	}
}

int TextAdd(const Str *Array, uint32_t Size, const char *Path)
{
	HANDLE hFile = CreateFile(Path, GENERIC_WRITE, FILE_SHARE_READ, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return OPENING_ERROR;
	DWORD pos = SetFilePointer(hFile, 0, NULL, FILE_END), dwTemp = 0;
	if (pos == INVALID_SET_FILE_POINTER)
		return OPENING_ERROR;
	for (uint32_t i = 0; i < Size; i++)
	{
		WriteFile(hFile, Array[i].Start, Array[i].Size, &dwTemp, NULL);
		if (Array[i].Size != dwTemp)
		{
			CloseHandle(hFile);
			return WRITING_ERROR;
		}
		WriteFile(hFile, "\r\n", 2, &dwTemp, NULL);
		if (2 != dwTemp)
		{
			CloseHandle(hFile);
			return WRITING_ERROR;
		}
	}
	WriteFile(hFile, "\r\n", 2, &dwTemp, NULL);
	if (2 != dwTemp)
	{
		CloseHandle(hFile);
		return WRITING_ERROR;
	}
	CloseHandle(hFile);
	return 0;
}

bool Comp(const Str& lft, const Str& rht)
{
	int i = lft.Size - 1, j = rht.Size - 1;
	while (true)
	{
		if (i < 0)
			return 0;
		if (j < 0)
			return 1;
		if (*(lft.Start + i) != *(rht.Start + j))
			return (*(lft.Start + i) > *(rht.Start + j));
		i--;
		j--;
	}
}

template<typename T>
void MyQSort(T *Data, uint32_t Size)
{
	if (Size > 1)
	{
		uint32_t Pos = Size / 2;
		uint32_t Counter = 0;

		for (uint32_t i = 0; i < Size; i++)
			if (Data[Pos] > Data[i])
				Counter++;

	
		std::swap(Data[Pos], Data[Counter]);
		Pos = 0;

		for (uint32_t i = 0; i < Size; i++)
			if (Data[Counter] > Data[i])
			{
				if (Pos != i)
					std::swap(Data[Pos], Data[i]);
				Pos++;
			}
		MyQSort(Data, Counter);
		MyQSort(Data + Counter + 1, Size - Counter - 1);
	}
}
