#include "BinaryTree.h"
#include <cstdio>
#include <cstring>

int CreateAkinatorNode(Tree*, tdata* name, tdata* diff);
int CreateLeave(Tree*, tdata*);

int Create(FILE*)
{
	return 0;
}

int Akinator(Tree* Tr)
{
	char c = 'e';
	std::cout << Tr->Data << "?\n";
	do
		std::cin >> c;
	while ((c != 'n') && (c != 'y'));
	if ((c == 'y') && (Tr->Right != nullptr))
		return Akinator(Tr->Right);
	else if ((c == 'n') && (Tr->Left != nullptr))
		return Akinator(Tr->Left);
	else if (c == 'n')
	{
		tdata name[256] = { 0 };
		tdata differ[256] = { 0 };
		std::cout << "What is it?\n";
		std::cin >> name;
		std::cout << "What the difference between " << name << " and " << Tr->Data << "\n";
		std::cin >> differ;
		tdata *BufName = (tdata*)malloc(sizeof(tdata) * (strlen(name) + 1));
		strcpy(BufName, name);
		tdata *BufDiff = (tdata*)malloc(sizeof(tdata) * (strlen(differ) + 1));
		strcpy(BufDiff, differ);
		return CreateAkinatorNode(Tr, BufName, BufDiff);
	}
	return 0;
}

int CreateAkinatorNode(Tree *tr, tdata * nam, tdata * diff)
{
	tr->Left = (Tree*)malloc(sizeof(Tree));
	CreateLeave(tr->Left, tr->Data);
	tr->Right = (Tree*)malloc(sizeof(Tree));
	CreateLeave(tr->Right, nam);
	tr->Data = diff;
	return 0;
}

int CreateLeave(Tree*tr, tdata*dat)
{
	tr->Data = dat;
	tr->Left = nullptr;
	tr->Right = nullptr;
	return 0;
}
