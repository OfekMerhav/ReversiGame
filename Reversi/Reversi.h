#pragma once
#include <stdio.h>

#define GAMESIZE 8
#define MATRIXSIZE GAMESIZE+2
#define VECTORSIZE 8

#define RESET   "\033[0m"
#define REDCOLOR     "\033[31m"      /* Red */
#define GREENCOLOR   "\033[32m"      /* Green */
#define REDC   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

//Enum for direction
typedef enum { 
	UP,
	UPRIGHT, 
	RIGHT, 
	DOWNRIGHT, 
	DOWN, 
	DOWNLEFT, 
	LEFT, 
	UPLEFT} Direction;

//enum of players 
typedef enum Player
{
	GREEN,
	RED,
	EMPTY,
	POSIBBLEMOVE
	
}Player,Color;

//struct of represnts a vector
typedef struct Vector
{
	int row, col;
}Vector;

//Status enum
typedef enum {
	
   CHANGE,
   DONTCHANGE

}Status;


void MainGame();

void ChangePlayer(Player* player);

void InitBoard(Color Board[][MATRIXSIZE]);

void ClearAllPossibleMoves(Color Board[][MATRIXSIZE]);

void InitVector(Vector vector[VECTORSIZE]);

int GetPosition(int* row, int* col, Color Board[][MATRIXSIZE], Player CurrentPlayer, Vector vector[]);

int DoesHaveATurn(Color Board[][MATRIXSIZE], Player CurrentPlayer, Vector vector[]);

int CheckValidInput(int row, int col, Color Board[][MATRIXSIZE]);

int DoAnyBridge(Player CurrentPlayer, Color Board[MATRIXSIZE][MATRIXSIZE], int row, int col, Vector vector[], Status CanChange);

int CheckBridgeInDirection(Player CurrentPlayer, Color Board[MATRIXSIZE][MATRIXSIZE], int x, int y, Direction direction, Vector vector[], Status CanChange);

int SetPossibleMoves(Color Board[][MATRIXSIZE], Player CurrentPlayer, int row, int col, Vector vector[]);

int BoardIsFull(Color Board[][MATRIXSIZE]);

Player SearchForWinner(Color Board[][MATRIXSIZE]);

void PrintBoard(Color Board[MATRIXSIZE][MATRIXSIZE]);




