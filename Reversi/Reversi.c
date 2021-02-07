#include "Reversi.h"

/*
* FUNCTION STATEMENT: The main function that manage the game
* INPUT STATEMENT: None
* OUTPUT STATEMENT: None
*/
void MainGame()
{
	//Start Player is green
	Player CurrentPlayer = GREEN;
	//Represents the board of the game
	Color Board[MATRIXSIZE][MATRIXSIZE];
	//Array of vectors when each vector represents a direction
	Vector vector[8];
	int row, col;

	//Intialization of the board matrix and vectors array
	InitBoard(Board);
	InitVector(vector);
	PrintBoard(Board);

	//Running until the game is over
	while(1)
	{
		//checks if atleast one of the players can make his turn
		if (DoesHaveATurn(Board, GREEN, vector) > 0 || DoesHaveATurn(Board, RED, vector) > 0)
		{
			//Clears the printing o
			ClearAllPossibleMoves(Board);
			if (!BoardIsFull(Board) && DoesHaveATurn(Board, CurrentPlayer, vector) > 0)
			{

				PrintBoard(Board);
				for (int i = 0; i < 3; i++)
				{
					printf("Its player number -- %d -- turn\n", CurrentPlayer);
					if (GetPosition(&row, &col, Board, CurrentPlayer, vector) == 1) {
						break;
					}

				}
				

			}
			else if (!BoardIsFull(Board))
			{
				int winner = SearchForWinner(Board);
				printf("The Winner is: %d", winner);
				PrintBoard(Board);
				break;
			}
			//posibilty of adding a clue to the user after not succeeding 3 times
			ChangePlayer(&CurrentPlayer);
		}
		//when no player has a possibility to make a turn, game is over
		else
		{
			//gets the winner number
			int winner = SearchForWinner(Board);
			//print board
			PrintBoard(Board);
			//print the winner
			if(winner == 0)
				printf("The Winner is:GREEN");
			else
				printf("The Winner is:RED");
			
			//break the main loop of the game
			break;
		}

	}
	


}

/*
* FUNCTION STATEMENT: Intializing the game board matrix
* INPUT STATEMENT: None
* OUTPUT STATEMENT: None
*/
void InitBoard(Color Board[][MATRIXSIZE])
{
	for (int i = 0; i < MATRIXSIZE; i++)
	{
		for (int j = 0; j < MATRIXSIZE; j++)
		{
			Board[i][j] = EMPTY;
		}
	}
	//Sets the start positions of the players tools
	Board[4][5] = GREEN;
	Board[4][4] = RED;
	Board[5][5] = RED;
	Board[5][4] = GREEN;
}

/*
* FUNCTION STATEMENT: Intializing the vectors
* INPUT STATEMENT: Array of vectors,Each vector represents a direction
* OUTPUT STATEMENT: None
*/
void InitVector(Vector vector[VECTORSIZE])
{
	vector[UP].row = -1;
	vector[UP].col = 0;
	vector[UPRIGHT].row = -1;
	vector[UPRIGHT].col = 1;
	vector[RIGHT].row = 0;
	vector[RIGHT].col = 1;
	vector[DOWNRIGHT].row = 1;
	vector[DOWNRIGHT].col = 1;
	vector[DOWN].row = 1;
	vector[DOWN].col = 0;
	vector[DOWNLEFT].row = 1;
	vector[DOWNLEFT].col = -1;
	vector[LEFT].row = 0;
	vector[LEFT].col = -1;
	vector[UPLEFT].row = -1;
	vector[UPLEFT].col = -1;
}

/*
* FUNCTION STATEMENT: Clears all the marked possible moves
* INPUT STATEMENT: matrix -> Board
* OUTPUT STATEMENT: None
*/
void ClearAllPossibleMoves(Color Board[][MATRIXSIZE])
{
	for (int i = 1; i <= GAMESIZE; i++)
	{
		for (int j = 1; j <= GAMESIZE; j++)
		{
			if (Board[i][j] == POSIBBLEMOVE)
				Board[i][j] = EMPTY;

		}
	}
}

/*
* FUNCTION STATEMENT: change the the CurrentPlayer to his opponnent
* INPUT STATEMENT: Pointer to the CurrentPlayer of the main function
* OUTPUT STATEMENT: None
*/
void ChangePlayer(Player* Currentplayer)
{
	switch (*Currentplayer)
	{
		case(GREEN):
			*Currentplayer = RED;
			break;
		case(RED):
			*Currentplayer = GREEN;
			break;

	}
}

/*
* FUNCTION STATEMENT: Intializing the vectors
* INPUT STATEMENT: Array of vectors,Each vector represents a direction
* OUTPUT STATEMENT: True if the input was legal else False
*/
int GetPosition(int* row,int* col, Color Board[][MATRIXSIZE], Player CurrentPlayer,Vector vector[])
{
	//Gets the players move
	printf("Insert row:\n");
	scanf_s("%d",row);
	printf("Insert col:\n");
	scanf_s("%d", col);

	//checks if the move is legal and if any bridge has been made
	if (CheckValidInput(*row, *col, Board) && DoAnyBridge(CurrentPlayer, Board, *row, *col, vector,CHANGE)) {
		Board[*row][*col] = CurrentPlayer;
		return 1;
	}
	printf("Wrong input\n");
	//if not return False
	return 0;
	

}

/*
* FUNCTION STATEMENT: Checks wether the player has a bridge to do and mark it as possible in the board
* INPUT STATEMENT: game board,the current player and vector array
* OUTPUT STATEMENT: have or not a possible turn
*/
int DoesHaveATurn(Color Board[][MATRIXSIZE],Player CurrentPlayer, Vector vector[])
{
	int counter=0;
	for (int i = 1; i <= GAMESIZE; i++)
	{
		for (int j = 1; j <= GAMESIZE; j++)
		{
			if (Board[i][j] != EMPTY && Board[i][j] != CurrentPlayer && Board[i][j] != POSIBBLEMOVE) {
				counter += SetPossibleMoves(Board, CurrentPlayer, i, j, vector);
			}
				
		}
	}
	return counter;
}
/*
* FUNCTION STATEMENT: Check if the input is in the board bounds and is made on a valid position
* INPUT STATEMENT: row,col and board
* OUTPUT STATEMENT: True for a valid location and False for invalid
*/
int CheckValidInput(int row, int col, Color Board[][MATRIXSIZE])
{
	//if the position is out of the board bounds return false
	if (row < 1 || row > GAMESIZE || col < 1 || col > GAMESIZE)
	{
		return 0;
	}
	//if the postion in the board is already taken return flase
	if (Board[row][col] != EMPTY && Board[row][col] != POSIBBLEMOVE)
	{
		return 0;
	}
	//return true
	return 1;
}
/*
* FUNCTION STATEMENT:Runs over all the directions and execute the bridge flipper if the status allows
* INPUT STATEMENT: Current player,board,row,col,vectors array,status->can change or not
* OUTPUT STATEMENT: True if can do at least one bridge
*/
int DoAnyBridge(Player CurrentPlayer, Color Board[MATRIXSIZE][MATRIXSIZE], int row, int col, Vector vector[],Status CanChange) {

	int CanDoBridge = 0;
	if (CheckValidInput(row, col, Board))
	{
		for (int direction = 0; direction < 8; direction++)
		{
			if (CheckBridgeInDirection(CurrentPlayer, Board, col, row, direction, vector,CanChange) > 1)CanDoBridge++;
		}
		
	}
	return CanDoBridge;
}

/*
* FUNCTION STATEMENT: Recursive function that flip the bridge if the status in change 
* INPUT STATEMENT: Current plyaer,board,x,y,direction,vectors array,status ->canChange
* OUTPUT STATEMENT: Return if a bridge can be flipped in the given direction
*/
int CheckBridgeInDirection(Player CurrentPlayer, Color Board[MATRIXSIZE][MATRIXSIZE], int x, int y, Direction direction, Vector vector[], Status CanChange) {
	int IsBridge = 0;
	//moving the position
	x += vector[direction].col;
	y += vector[direction].row;
	if (Board[y][x] == EMPTY || Board[y][x] == POSIBBLEMOVE) {
		return 0;
	}
	if (Board[y][x] == CurrentPlayer) {
		return 1;
	}
	if(CanChange == CHANGE)
		Board[y][x] = CurrentPlayer;
	IsBridge = CheckBridgeInDirection(CurrentPlayer, Board, x, y, direction,vector,CanChange) == 0 ? 0 : 2;
	if (CanChange == CHANGE)
		if (!IsBridge) {
			Board[y][x] = Board[y][x] == GREEN ? RED : GREEN;
		}
	return IsBridge;
}

/*
* FUNCTION STATEMENT: Sets all the Possible moves a given position has
* INPUT STATEMENT: board,currentPlayer,row,col,vectors array
* OUTPUT STATEMENT: return if there is any possible move
*/
int SetPossibleMoves(Color Board[][MATRIXSIZE],Player CurrentPlayer, int row, int col, Vector vector[])
{
	//counts the number of possible positions
	int counter = 0;
	int j, i;

	//runs over the rows around the corrent position
	for (i = row - 1; i < row + 2; i++)
	{
		//in case the row in out of bounds,dont keep on checking the position
		if (i >= 1 && i <= GAMESIZE) {
			//runs over the column around the corrent position
			for (j = col - 1; j < col + 2; j++)
			{
				//in case the column in out of bounds,dont keep on checking the position
				if (j >= 1 && j <= GAMESIZE)
				{
					//if the position is empty:
					if (Board[i][j] == EMPTY) {
						//if its not the given position
						if (!(i == row && j == col)) {	
							//Execute do a bridge of the position
							if (DoAnyBridge(CurrentPlayer, Board, i, j, vector,DONTCHANGE) > 0)
							{
								//if a bridge was made increase the counter and mark the position as possible move
								counter++;
								Board[i][j] = POSIBBLEMOVE;
							}
							
						}
					}
				}
			}
		}
	}
	return counter;

}

/*
* FUNCTION STATEMENT: Checks if the board is full
* INPUT STATEMENT: game board
* OUTPUT STATEMENT: Return True/False if the board is full/Not full
*/
int BoardIsFull(Color Board[][MATRIXSIZE])
{
	for (int i = 1; i <= GAMESIZE; i++)
	{
		for (int j = 1; j <= GAMESIZE; j++)
		{
			if (Board[i][j] == EMPTY) return 0;
		}
	}
	return 1;
}

/*
* FUNCTION STATEMENT: Counts each player tool on the board
* INPUT STATEMENT: game board
* OUTPUT STATEMENT: return who is the player with more of his tools in the board
*/
Player SearchForWinner(Color Board[][MATRIXSIZE])
{
	int green=0, red=0;
	for (int i = 1; i <= GAMESIZE; i++)
	{
		for (int j = 1; j <= GAMESIZE; j++)
		{
			if (Board[i][j] == GREEN)
				green++;
			else if(Board[i][j] == RED)
				red++;
		}
	}
	return green > red ? GREEN : RED;
}


/*
* FUNCTION STATEMENT: Prints the game board
* INPUT STATEMENT: board
* OUTPUT STATEMENT: None
*/
void PrintBoard(Color Board[MATRIXSIZE][MATRIXSIZE])
{
	int i = 0, j = 0;
	printf("\n");
	printf("      ");
	for (j = 1; j <= GAMESIZE; j++)
	{
		printf("  %d   ", j);
	}
	printf("\n      ");
	for (j = 1; j <= GAMESIZE; j++)
	{
		for(j = 1; j <= GAMESIZE;j++)
			printf("_____ ");

		for (i = 1; i <= GAMESIZE; i++)
		{
			printf("\n     |");
			for (j = 1; j <= GAMESIZE; j++)
			{
				printf("     |");
			}
			printf("\n  %d  |", i);
			for (j = 1; j <= GAMESIZE; j++)
			{
				//print if empty
				if (Board[i][j] == EMPTY)
					printf("  %c  ", ' ');
				else
					//print if player 0
					if (Board[i][j] == GREEN)
						printf(GREENCOLOR "  *  " RESET);
					//print if player1
					else if (Board[i][j] == RED)
						printf(REDCOLOR"  *  " RESET);
					else
						printf("  %c  ", '*');
				printf("|");
			}
			printf("\n     |");
			for (j = 1; j <= GAMESIZE; j++)
			{
				printf("_____|");
			}
		}
		printf("\n");
		
	}
}
