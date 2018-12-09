#pragma warning(disable : 4996)
#include <iostream>
#include <cstdint>
#include "BinaryTree.h"


int main()
{
	Tree Tr;

	Tr.Data = (tdata*)malloc(sizeof(tdata) * (strlen("You") + 1));
	strcpy(Tr.Data, "You");
	Tr.Left = nullptr;
	Tr.Right = nullptr;
	while (true)
	{
		Akinator(&Tr);
	}
	return 0;
}
