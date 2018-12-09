#ifndef BINARYTREE_H
#define BINARYTREE_H
#pragma warning(disable : 4996)
#include <iostream>

using tdata = char;

struct Tree
{
	Tree* Left;
	Tree* Right;
	tdata* Data;
};

int Akinator(Tree*);
int Create(FILE*);

#endif // !BINARYTREE_H
