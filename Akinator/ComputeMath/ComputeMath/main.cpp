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
	char chr = '\0';
	tdata buf[256] = { 0 };
	while (chr != 'e')
	{
		std::cout << "Commands: [a]- Akinator, [d]- Descriptor, [e]- exit, [c]- dump" << std::endl;
		std::cin >> chr;
		switch (chr)
		{
			case 'a':
				Akinator(Tr);
				break;
			case 'd':
				std::cin >> buf;
				ObjectDescriptor(Tr, buf);
				break;
			case 'c':
				DotDump(Tr, "C:/Users/Иван/source/repos/ComputeMath/ComputeMath/Dump.gv");
				break;
			default:
				break;
		}

	}
	TreeDestruct(Tr);
	return 0;
}