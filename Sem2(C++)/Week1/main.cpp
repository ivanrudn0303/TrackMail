#include "Stack.h"
#include <iostream>

int main()
{
	Stack St(5);
	St.Push(5.0);
	double a = 0.0;
	St.Pop(&a);
	std::cout << a << std::endl;
	St.Erase();
	St.Push(4.0);
	St.Pop(&a);
	std::cout << a << std::endl;
	St.StackDump("logfile.txt");
	system("PAUSE");
	return 0;
}
