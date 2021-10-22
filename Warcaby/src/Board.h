#pragma once
#include "Log.h"
class Board
{
private:
	int board[64];
	 
//private:
public:
	bool Limit(int position)
	{
		
		bool lowerBound = ((int)(A1 - 1) < position);
		bool upperBound = ((int)(A1 + 64) > position);
		return (lowerBound!=false && upperBound!=false);

	}
	int modulo(int* pos, int vaule)
	{
		return (int)((int*)((char*)pos - (char*)A1)) % 32;
	}
public:
	int* A1= &board[0];

	Board()
	{
		
		
		LOG("Board Created");


	}
	~Board()
	{
		LOG("Board Destroyed");
	}
	void FillRow(int y, int symbol)
	{
		for (int* i = A1 + 8 * y; i < A1 + 8 * (y + 1); i++)
		{
			*i = symbol;
		}
	}
	void FillRow(int y, int symbol, int spacing, int offset)
	{
		for (int* i = (A1 + 8 * y + offset); i < (A1 + 8 * (y + 1)); i += (1 + spacing))
		{
			*i = symbol;
		}
		LOG("Row Filled");
	}
	void Start()
	{
		for (int i = 0; i < 8; i++)
			FillRow(i, 0);
		FillRow(0, 1, 1, 0);
		FillRow(1, 1, 1, 1);
		FillRow(2, 1, 1, 0);
		//FillRow(5, 1, 1, 1);
		//FillRow(6, 2, 1, 0);
		FillRow(5, 2, 1, 1);
		FillRow(6, 2, 1, 0);
		FillRow(7, 2, 1, 1);
		LOG("Board Started");
	}
	void Display()
	{
		//system("cls");
		for (int* i = A1; i < A1 + 64; i++)
		{
			if (!((int)(i - A1) % 8))
				std::cout << std::endl;
			std::cout << *i;
		}
		LOG("\nBoard Displayed");
	}
	int* PositionConv(int &X, char &Y)
	{		
			return A1 + (Y-65) +( X-1)*8;
	}
	void ChangeSymbol(int* position, int symbol)
	{
		*position = symbol;
	}
	int RelativePosition(int* posOnBoard)
	{
		if (posOnBoard)
			return (int)((char*)posOnBoard - (char*)A1);
		else
			return -1;
	}
	//side 0,left 1,right 
	//upOrDown 0,up 1,down
	int* DiagonalMovement(int* position, bool side, bool upOrDown) //TODO fix modulos you piece of shit 
	{

		int offset = 28 + side * 8 *!upOrDown + !side * 8 * upOrDown;
		//int* check = (int*)(
		//	((((int)((char*)position + offset))) * (upOrDown ))
		//		+ ((int)((char*)position - offset)) * (!upOrDown ));
		int* LOCALcheck = NULL;
		if (upOrDown)
		{
			LOCALcheck = (int*)((char*)position + offset);
		}
		else
		{
			LOCALcheck = (int*)((char*)position - offset);
		}
		
			if (Limit((int)LOCALcheck) && (abs(modulo(LOCALcheck, 32) - modulo(position, 32)) < 5))
			{
				return LOCALcheck;
			}
			else
			{
				return 0;
			}
		

	

		
	}
	void zeroMiddle(int* pos, int* target)

	{
		long long middle  = ((long long)pos + (long long)target) / 2;
		*(int*)middle = 0;
	}


	


};