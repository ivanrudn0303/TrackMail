#pragma warning(disable : 4996)
#include <iostream>
#include <cstdint>
#include "BinaryTree.h"


int main()
{
	Tree *Tr = (Tree*)calloc(1, sizeof(Tree));
	TreeCreate(Tr, "C:/Users/����/source/repos/ComputeMath/ComputeMath/text.txt");
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