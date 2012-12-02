#include <iostream>
#include <string>
#include <cstdlib>
#include <list>
#include "tree.h"
#include <vector>

using namespace std;

Tree::Tree()
{
	turn = 1;
	int i, j = DIMENSION;

	for( i=0; i < DIMENSION; i++ )
		for( j=0; j < DIMENSION;j++ )
			board[i][j] = EMPTY;

	rootNode = NULL;
	children = new LinkList<Tree*>;

	char turnSym;
	if(turn == 1)
		turnSym = X;
	else
		turnSym = O;

	for( i=0; i < DIMENSION; i++ )
		for( j=0; j < DIMENSION;j++ )
		{
			//cout <<"i:" << i << "  j:" << j << endl; 

			board[i][j] = turnSym;
			//displayBoard();
			children->insert(new Tree(turn,board)); 
			//X Goes first
			//Send a board with each of the 9 starting moves			
			board[i][j] = EMPTY;
		}
	//cout << "Tree made." << endl;
	gameSelection();
}

Tree::Tree( int newTurn, char newBoard[DIMENSION][DIMENSION] )
{
	turn = newTurn;
	char turnSym;	

	if(turn == 1)
	{
		turnSym = O;
		turn = 2;
	}
	else
	{
		turnSym = X;
		turn = 1;
	}
		
	children = new LinkList<Tree*>;

	int i, j = DIMENSION;
	
	for( i=0; i < DIMENSION; i++ )
	{
		for( j=0; j < DIMENSION;j++ )
		{
			board[i][j] = newBoard[i][j];
		}
	}

	//displayBoard();
	//Create the rest of the trees, as long as the current one isnt a winner	
	if(!isWinner(this))
	{
		//cout << "not a winner" << endl;
		for( i=0; i < DIMENSION; i++ )
			for( j=0; j < DIMENSION;j++ )
			{
				if(board[i][j] == EMPTY)
				{
					board[i][j] = turnSym;
					children->insert(new Tree(turn,board));
					board[i][j] = EMPTY;
				}
			}
	}
	
	rootNode = NULL;	
}

Tree::~Tree()
{
}

void Tree::addMove()
{
	int playBoard[DIMENSION][DIMENSION];
	int iter = 0;
	
	for( int i=0; i < DIMENSION; i++ )
	{
		for( int j=0; j < DIMENSION; j++ ) //do this three times for that row
		{
			iter++;
			playBoard[i][j] = iter;
		}
	}

	char symbol = ' ';

	while(!gameOver())
	{
		compMove(children);
		cout << "CURENT BOARD:"<< endl;
		displayBoard();
		if(!isWinner(this))
		{
			if(turn == 1)
				symbol = O;
			else
				symbol = X;
	
			cout << endl;
			cout << " " << playBoard[0][0] << " | " << playBoard[0][1] << " | " << playBoard[0][2] << endl;
			cout << "-----------" << std::endl;
			cout <<  " " << playBoard[1][0] << " | " << playBoard[1][1] << " | " << playBoard[1][2] << endl;
			cout << "-----------" << std::endl;
			cout <<  " " << playBoard[2][0] << " | " << playBoard[2][1] << " | " << playBoard[2][2] << endl;
			cout << endl;

			cout << "Where would you like to play " << symbol<< " : " << endl; 
			int choice = 0;		
			cin >> choice;

			updateBoard(choice, symbol);

			system("cls");
		}
	}
	//cout << "Game complete!!!" << endl;
}

void Tree::compMove(LinkList<Tree*> *&children )
{
	int tmp = 0;
	bool cont = true;
	//cout << "computer making choice" << endl;
	vector<int> choices;
	vector<Tree> tempChildren;
	
	LinkList<Tree*> *tempList = children;

	Tree *temp = tempList->getNext()->getData();
	
	while(cont)
	{
		choices.push_back(miniMax(0, temp));

		if (tempList->getNext() != NULL)
		{
			tempList = tempList->getNext();

			if( tempList->getNext()!= NULL)
				temp = tempList->getNext()->getData();
			else
				cont = false;
		}
		else
			cont = false;
	}
	
	//cout << "VECTOR: " << endl;
	
	int choice = 0;

	for( int i = 0; i < choices.size(); i++ ) 
	{		
		//cout << i << " : " << choices.at(i) << endl;
		if(choices.at(i) > choice)
		{
			choice = choices.at(i);
		}
	}	
	
	LinkList<Tree*> *tempList2 = children;
	Tree *bestTree = tempList2->getNext()->getData();
	
	cont = true;
	int select = 0;
	
	while(cont)
	{
		select++;
		//cout <<"select + "<< select << endl;
		if(miniMax(0,bestTree) == choice)
		{
			//bestTree->displayBoard();
			//cout << "selecting: " << select << endl;
			//cout << "minmax:" << miniMax(0,bestTree)<< endl;
			cont = false;
			int i, j = DIMENSION;

			for( i=0; i < DIMENSION; i++ )
				for( j=0; j < DIMENSION;j++ )
					board[i][j] = bestTree->board[i][j];

			//cout << "board" <<board[0][0] << endl;
			children = bestTree->children;

		}
		else
		{
			if (tempList2->getNext() != NULL)
			{
				tempList2 = tempList2->getNext();

				if( tempList2->getNext()!= NULL)
					bestTree = tempList2->getNext()->getData();
				else
					cont = false;
			}
			else
				cont = false;	
		}

	}
}

//minMax - send this an int that is = to 0
//child - 1 of the 9 children
int Tree::miniMax(int minMax, Tree *child)
{
	Tree *temp = child;
	LinkList<Tree*> *tempList = temp->children;
	bool cont = true;
	//cout << "Current Board" << endl;

	//temp->displayBoard();
	
	if(!isWinner(temp)) //if not a game over board
	{	
		if(temp->children != NULL)//if it has children
		{
			while(cont)
			{
				if (tempList->getNext() != NULL)//if the next isnt NULL
				{
					tempList = tempList->getNext(); //set temp to the next

					if( tempList->getNext()!= NULL)
					{
						temp = tempList->getNext()->getData();						
						minMax = miniMax(minMax, temp);
					}
					else
						cont = false;
				}
				else
					cont = false;
			}

		}
	}
	else
	{		
		//cout << " ANYLYZ MINMAX" << endl;

		
		int canWin = 0;
		int canLose = 0;
		
		char play = ' ';
		char play2 = ' ';

		//theoretically, turn should currently be the computers move
		
		if(temp->turn == 1 )
		{
			play = X;
			play2 = O;
		}
		else
		{
			play = O;
			play2 = X;
		}
		
		//Check ROWS
		for( int i=0; i < DIMENSION; i++ )
		{
			for( int j=0; j < DIMENSION; j++ ) //do this three times for that row
			{
				if((temp->board[i][j] == play)||
					(temp->board[i][j] == EMPTY )) //if the spot is empty or the computers current X/O
					canWin++;	
				else
					canWin--;

				if((temp->board[i][j] == play2)||
					(temp->board[i][j] == EMPTY )) //if the spot is empty or the players current X/O
					canLose++;	
				else
					canLose--;
			}		
			//after checking that row if that row is potentially winnable by both sides
			if(canWin == 3)			// if computer can win using that row
			{
				minMax++;
			}
			else if(canLose == 3)		// if computer can lose using that row
			{
				minMax--;				
			}
		}

		canLose = 0;
		canWin = 0;

		//Check Columns			
		for( int i=0; i < DIMENSION; i++ )
		{
			for( int j=0; j < DIMENSION; j++ ) //do this three times for that row
			{
				if((temp->board[j][i] == play)||
					(temp->board[j][i] == EMPTY )) //if the spot is empty or the computers current X/O
					canWin++;	
				else
					canWin--;

				if((temp->board[j][i] == play2)||
					(temp->board[j][i] == EMPTY )) //if the spot is empty or the players current X/O
					canLose++;	
				else
					canLose--;
			}		
			//after checking that row if that row is potentially winnable by both sides
			if(canWin == 3)			// if computer can win using that row
			{
				minMax++;
			}
			else if(canLose == 3)		// if computer can lose using that row
			{
				minMax--;				
			}
		}

		//Check X formation
		if(
			(temp->board[0][0] == play) ||(temp->board[0][0] == EMPTY) &&
			(temp->board[1][1] == play) ||(temp->board[1][1] == EMPTY) &&
			(temp->board[2][2] == play) ||(temp->board[2][2] == EMPTY)

			)
			{minMax++;}	
		if(	
			(temp->board[0][2] == play) ||(temp->board[0][2] == EMPTY) &&
			(temp->board[1][1] == play) ||(temp->board[1][1] == EMPTY) &&
			(temp->board[2][0] == play) ||(temp->board[2][0] == EMPTY)
			)				
			{minMax++;}	

		if(
			(temp->board[0][0] == play2) ||(temp->board[0][0] == EMPTY) &&
			(temp->board[1][1] == play2) ||(temp->board[1][1] == EMPTY) &&
			(temp->board[2][2] == play2) ||(temp->board[2][2] == EMPTY)

			)
			{minMax--;}	
		if(	
			(temp->board[0][2] == play2) ||(temp->board[0][2] == EMPTY) &&
			(temp->board[1][1] == play2) ||(temp->board[1][1] == EMPTY) &&
			(temp->board[2][0] == play2) ||(temp->board[2][0] == EMPTY)
			)				
			{minMax--;}			
	}
	return minMax;
}

void Tree::displayBoard() 
{
	cout << endl;
	cout << " " << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << endl;
	cout << "-----------" << std::endl;
	cout <<  " " << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << endl;
	cout << "-----------" << std::endl;
	cout <<  " " << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << endl;
	cout << endl;
}

bool Tree::isOpen(int compMove)
{
	if((compMove == 1 )&&(board[0][0]) == EMPTY)
		return true;
	else if((compMove == 2) &&(board[0][1]) == EMPTY)
		return true;
	else if((compMove == 3) &&(board[0][2]) == EMPTY)
		return true;
	else if((compMove == 4) &&(board[1][0]) == EMPTY)
		return true;
	else if((compMove == 5) &&(board[1][1]) == EMPTY)
		return true;
	else if((compMove == 6) &&(board[1][2]) == EMPTY)
		return true;
	else if((compMove == 7) &&(board[2][0]) == EMPTY)
		return true;
	else if((compMove == 8) &&(board[2][1]) == EMPTY)
		return true;
	else if((compMove == 9) &&(board[2][2]) == EMPTY)
		return true;
	else
	{
		return false;
	}
}
void Tree::resetBoard()
{
	int i, j = DIMENSION;

	for( i=0; i < DIMENSION; i++ )
		for( j=0; j < DIMENSION;j++ )
			board[i][j] = EMPTY;
}


void Tree::updateBoard( int playerMove, const char playerSymbol) 
{
	//cout << "UPDATING BOARD1" << endl;
	bool boardChanged = true;

	if((playerMove == 1 )&&(board[0][0]) == EMPTY)
		board[0][0] = playerSymbol;
	else if((playerMove == 2) &&(board[0][1]) == EMPTY)
		board[0][1] = playerSymbol;
	else if((playerMove == 3) &&(board[0][2]) == EMPTY)
		board[0][2] = playerSymbol;
	else if((playerMove == 4) &&(board[1][0]) == EMPTY)
		board[1][0] = playerSymbol;
	else if((playerMove == 5) &&(board[1][1]) == EMPTY)
		board[1][1] = playerSymbol;
	else if((playerMove == 6) &&(board[1][2]) == EMPTY)
		board[1][2] = playerSymbol;
	else if((playerMove == 7) &&(board[2][0]) == EMPTY)
		board[2][0] = playerSymbol;
	else if((playerMove == 8) &&(board[2][1]) == EMPTY)
		board[2][1] = playerSymbol;
	else if((playerMove == 9) &&(board[2][2]) == EMPTY)
		board[2][2] = playerSymbol;
	else
	{
		system("cls");
		cout << "invalid choice" << endl;
		boardChanged = false;
	}

	
	if(boardChanged == false)
	{
		if(!isWinner(this))
		{
			cout << "CURRENT BOARD:" << endl;
			displayBoard();

			int playBoard[DIMENSION][DIMENSION];
			int iter = 0;
	
			for( int i=0; i < DIMENSION; i++ )
			{
				for( int j=0; j < DIMENSION; j++ ) //do this three times for that row
				{
					iter++;
					playBoard[i][j] = iter;
				}
			}

			char symbol = ' ';

			if(turn == 1)
				symbol = O;
			else
				symbol = X;
	
			cout << endl;
			cout << " " << playBoard[0][0] << " | " << playBoard[0][1] << " | " << playBoard[0][2] << endl;
			cout << "-----------" << std::endl;
			cout <<  " " << playBoard[1][0] << " | " << playBoard[1][1] << " | " << playBoard[1][2] << endl;
			cout << "-----------" << std::endl;
			cout <<  " " << playBoard[2][0] << " | " << playBoard[2][1] << " | " << playBoard[2][2] << endl;
			cout << endl;

			cout << "Where would you like to play " << symbol<< " : " << endl; 
			int choice = 0;		
			cin >> choice;

			updateBoard(choice, symbol);

			system("cls");
		}
	}
	else
	{
		cout << "TRUE" << endl;
		if(children->getNext() != NULL)
		{
			LinkList<Tree*> *temp = children;
			int isBoard = 0;
			for(int i =0; i < 9; i++)
			{
				//cout << i << endl;
				//temp->getNext()->getData()->displayBoard();
				for( int k=0;  k< DIMENSION; k++ )
				{
					for( int j=0; j < DIMENSION; j++ )
					{
						if(board[k][j] == temp->getNext()->getData()->board[k][j])
						{
						
							isBoard++;
						}
						else
						{
							isBoard--;
						}
					}
				}
				
				if (isBoard == 9) 
				{
					children = temp->getNext()->getData()->children;
					//cout << "hi there" << endl;
					
					//cout << "Child1" << endl;
					//children->getNext()->getData()->displayBoard();
					//cout << "child2" << endl;
					//children->getNext()->getNext()->getData()->displayBoard();
					//system("pause");
					break;
				}

				if(temp->getNext() != NULL)
				{
					isBoard = 0;
					temp = temp->getNext();
				}
			}
		}
		//cout << "UPDATING BOARD2" << endl;
	}
}
void Tree::gameSelection() 
{
	int input;

	cout << "1 - Play a Game Against the Computer"<< endl;

	cout << "2 - Quit the Program" << endl;
	cout <<"Enter Selection:"<< endl;

	cin >> input;

	if(input == 1)
		addMove();
	else if(input == 2)
		return;
}

bool Tree::gameOver()
{
	int over = 0;

	
	if ((board[0][0] == X && board[0][1] == X && board[0][2] == X) ||
		(board[1][0] == X && board[1][1] == X && board[1][2] == X) ||
		(board[2][0] == X && board[2][1] == X && board[2][2] == X) ||
		(board[0][0] == X && board[1][0] == X && board[2][0] == X) ||
		(board[0][1] == X && board[1][1] == X && board[2][1] == X) ||
		(board[0][2] == X && board[1][2] == X && board[2][2] == X) ||
		(board[0][0] == X && board[1][1] == X && board[2][2] == X) ||
		(board[0][2] == X && board[1][1] == X && board[2][0] == X)) 
	{	
		cout << "X WINS" << endl;
		return true;
	}
	else if((board[0][0] == O && board[0][1] == O && board[0][2] == O) ||
		(board[1][0] == O && board[1][1] == O && board[1][2] == O) ||
		(board[2][0] == O && board[2][1] == O && board[2][2] == O) ||
		(board[0][0] == O && board[1][0] == O && board[2][0] == O) ||
		(board[0][1] == O && board[1][1] == O && board[2][1] == O) ||
		(board[0][2] == O && board[1][2] == O && board[2][2] == O) ||
		(board[0][0] == O && board[1][1] == O && board[2][2] == O) ||
		(board[0][2] == O && board[1][1] == O && board[2][0] == O)) 
	{
		cout << "O WINS" << endl;	
		return true;
	}
	else
	{
		for( int i=0; i < DIMENSION; i++ )
			for( int j=0; j < DIMENSION; j++ ) //do this three times for that row
				if(board[j][i] != EMPTY)
					over++;

		if(over == 8)
		{
			cout << "The game board has been filled. Its a draw!" << endl;
			displayBoard();
			return true;
		}
	}

	return false;
}


bool Tree::isWinner(Tree *node)
 {
	 char symbol;
	if( node->turn == 1 )
		 symbol = X;
	 else
		 symbol = O;

	if ((node->board[0][0] == symbol && node->board[0][1] == symbol && node->board[0][2] == symbol) ||
		(node->board[1][0] == symbol && node->board[1][1] == symbol && node->board[1][2] == symbol) ||
		(node->board[2][0] == symbol && node->board[2][1] == symbol && node->board[2][2] == symbol) ||
		(node->board[0][0] == symbol && node->board[1][0] == symbol && node->board[2][0] == symbol) ||
		(node->board[0][1] == symbol && node->board[1][1] == symbol && node->board[2][1] == symbol) ||
		(node->board[0][2] == symbol && node->board[1][2] == symbol && node->board[2][2] == symbol) ||
		(node->board[0][0] == symbol && node->board[1][1] == symbol && node->board[2][2] == symbol) ||
		(node->board[0][2] == symbol && node->board[1][1] == symbol && node->board[2][0] == symbol)) 
	{	
		return true;
	}
	else 
	{
		return false;
	}
}



