#include <iostream>
#include "Processor.h"
#include "Compiler.h"

int main()
{
	printf("proc\n");
	Processor Proc;
	int Error = ProcessorCreate(&Proc);
	std::cout << Error << std::endl;
//	char Code[] = {20, 4, 0, 0, 0, 0, 0, 0, 0xc0, 0x3f, 12, 28, 16, 0, 0, 0, 32};
	char* Text = ReadData("Code.txt");
	uint32_t size;
//	char* Code = BinaryCreate(Text, &size);
//	Error = WriteData(Code, size, "code.o");
	char* Code = ReadData("Code.o");
	double data[] = { 1.5, 4.0 };
	double ans;
	ProcessorExec(&Proc, Code, 0, nullptr, &ans);
	std::cout << ans << std::endl;
	free(Text);
	free(Code);
	ProcessorDestruct(&Proc);
	system("PAUSE");
	return 0;
}
