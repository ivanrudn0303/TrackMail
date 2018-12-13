#include <iostream>
#include "Processor.h"
#include "Compiler.h"
#include "Disassembler.h"

int main()
{
	printf("proc\n");
	Processor Proc;
	std::cout << ProcessorCreate(&Proc) << std::endl;
//	char* Text = ReadData("Factorial.txt");
	char* Text = ReadData("SquareRoot.txt");
//	char* Text = ReadData("Dis.txt");
	uint32_t size;
	char* Code = BinaryCreate(Text, &size);
	int re = Disassemble(Code,"dis.txt", size);
//	WriteData(Code, size, "Factorial.o");
	free(Code);
	char* dis = ReadData("dis.txt");
	Code = BinaryCreate(dis, &size);
	double ans[3] = {NAN, NAN, NAN};
	std::cout << ProcessorExec(&Proc, Code, 0, nullptr, nullptr) << std::endl;
	free(dis);
	free(Text);
	free(Code);
	ProcessorDestruct(&Proc);
	system("PAUSE");
	return 0;
}