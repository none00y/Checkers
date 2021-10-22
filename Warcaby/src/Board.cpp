#pragma once
#include "Log.h"
class Board
{
private:
	int board[64];
public:
	int* A1;
	Board()
	{
		A1 = &board[0];
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
		for (int* i = (A1 + 8 * y + offset); i < A1 + 8 * (y + 1); (i + 1 + spacing))
		{
			*i = symbol;
		}
	}
	void Start()
	{
		FillRow(1, 2, 1, 1);
	}
	void Display()
	{
		for (int* i = A1; i < A1 + 64; i++)
		{
			if (!((int)(i - A1) % 8))
				std::cout << std::endl;
			std::cout << *i;
		}
	}

};
