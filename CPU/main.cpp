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
	double in[3];
	std::cin >> in[0] >> in[1] >> in[2];
	std::cout << ProcessorExec(&Proc, Code, 3, in, ans) << std::endl;
	std::cout <<"roots: "<< ans[0] << std::endl <<ans[1] << " " << ans[2] <<std::endl;
	free(dis);
	free(Text);
	free(Code);
	ProcessorDestruct(&Proc);
	system("PAUSE");
	return 0;
}
