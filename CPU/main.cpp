#include <iostream>
#include "Processor.h"
#include "Compiler.h"

int main()
{
	printf("proc\n");
	Processor Proc;
	std::cout << ProcessorCreate(&Proc) << std::endl;
	char* Text = ReadData("Factorial.txt");
	uint32_t size;
	char* Code = BinaryCreate(Text, &size);
	WriteData(Code, size, "Factorial.o");
	free(Code);
	Code = ReadData("Factorial.o");
	double data[] = { 1.5, 4.0 };
	double ans;
	double in;
	std::cin >> in;
	std::cout << ProcessorExec(&Proc, Code, 1, &in, &ans) << std::endl;
	std::cout << ans << std::endl;
	free(Text);
	free(Code);
	ProcessorDestruct(&Proc);
	system("PAUSE");
	return 0;
}
