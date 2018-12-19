#ifndef BINARYTREE_H
#define BINARYTREE_H
#pragma warning(disable : 4996)
#include <iostream>

#define DEBUG_BINARY_TREE

using tdata = char;

struct Tree
{
	Tree* Left;
	Tree* Right;
#ifdef DEBUG_BINARY_TREE
	Tree* Top;
#endif // !DEBUG_BINARY_TREE
	tdata* Data;
};

int Akinator(Tree*);
Tree* TreeCreate(int*, const char*);
int TreeDestruct(Tree*);
int ObjectDescriptor(const Tree*, const tdata*);
int DotDump(const Tree*, FILE*);

#ifdef DEBUG_BINARY_TREE
int TreeError(const Tree*);
#endif

#endif // !BINARYTREE_H
