#pragma warning(disable : 4996)
#include <iostream>
#include <cstdint>
#include "BinaryTree.h"


int main()
{
	int res;
	Tree *Tr = TreeCreate(&res, "C:/Users/Иван/source/repos/ComputeMath/ComputeMath/text.txt");
	if (res)
		return res;
	int i = 0;
	ObjectDescriptor(Tr, "Ilya");
	ObjectDescriptor(Tr, "rock");
	FILE *f = fopen("C:/Users/Иван/source/repos/ComputeMath/ComputeMath/Dump.gv", "w");
	std::cout << "Dump Code Error Tree " << DotDump(Tr, f) << std::endl;
	fclose(f);
	std::cout << TreeError(Tr) << std::endl;
//	Tr.Right->Right->Top = &Tr;
//	std::cout << TreeError(&Tr) << std::endl;
//	while (true)
//	{
		Akinator(Tr);
		Akinator(Tr);
//	}
	TreeDestruct(Tr);
	return 0;
}