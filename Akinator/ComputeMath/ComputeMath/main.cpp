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
	std::cout << TreeError(Tr) << std::endl;
//	Tr.Right->Right->Top = &Tr;
//	std::cout << TreeError(&Tr) << std::endl;
//	while (true)
//	{
		Akinator(Tr);
//	}
	TreeDestruct(Tr);
	return 0;
}