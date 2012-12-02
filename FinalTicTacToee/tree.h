#pragma once
#include "newLinkedlist.h"
#include <iostream>
#include <string>
#include <list>

using namespace std;

const char X = 'x', O = 'o', EMPTY = ' ';
const int DIMENSION = 3, INFINITY = 10000;

class Tree
{
public:
	Tree(); //makes the Initial tree, then calls its other constructor to build the branches
	Tree( int newTurn, char newBoard[DIMENSION][DIMENSION] );
	~Tree();

	void addMove();
	void compMove(LinkList<Tree*> *&children );
	void updateBoard( int playerMove, const char playerSymbol);
	bool isWinner(Tree *node);
	bool gameOver();
	void gameSelection();
	void displayBoard();
	void resetBoard();

private:

	char board[DIMENSION][DIMENSION];
	bool isOpen(int compMove);
	int moveValue;
	int turn;
	bool winningBoard;
	
	Tree *rootNode;
	LinkList<Tree*> *children;

	int miniMax(int minMax, Tree *children);

};



	


