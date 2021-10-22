#pragma once

#include <vector>
class Player
{
private:
public:
	std::vector<int*> unitPositions;
	struct hitPosition
	{
		int* PlayerPosition;
		int* HitPosition;
	};
	const int identity; // 0 - neutral,1 - playerOne,2 -playerTwo
	bool identityCheck = identity == 1;

	std::vector<hitPosition> possibleHits;
	Player(int identity)
		:identity(identity)
	{
		LOG("Player Created");
		LOG(identity);

	}
private:
public:
	void upgrade(int* A1)
	{
		for (int* units : unitPositions)
		{
			switch (identity)
			{
			case 1:
				if (units > (A1 + 55) && *units < 3)
				{
					*units += 2;
				}
				break;
			case 2:
				if (units < (A1 + 8) && *units < 3)
				{
					*units += 2;
				}
				break;
			}
		}
	}
	void CountUnits(Board& board)
	{
		unitPositions.clear();
		{
			for (int* i = board.A1; i < board.A1 + 64; i=(int*)((char*)i+4))
			{
				if (identity == *i || identity + 2 == *i )
				{

					unitPositions.push_back(i);

				}

			}

		}

	}
	void Move(int* position,int* target)
	{
			
			*target = *position;
			*position = 0;
		
		
		
	}
	bool HitPossibility(Board& board)
	{
		possibleHits.clear();
		for (int* units : unitPositions)
		{
			
			
			bool side = 0; 
			int* enemy = board.DiagonalMovement(units, side, identityCheck);
			int* check = board.DiagonalMovement(enemy, side, identityCheck);
			if (check!=nullptr&&*check ==0&&*enemy!=identity&&*enemy!=0 && *enemy != identity+2)
			{
				hitPosition temp = { units,check };
				possibleHits.push_back(temp);
				
			

			}
			side = 1;
			enemy = board.DiagonalMovement(units, side, identityCheck);
			check = board.DiagonalMovement(board.DiagonalMovement(units, side, identityCheck), side, identityCheck);
			if (check != nullptr && *check == 0 && *enemy != identity && *enemy != 0 && *enemy != identity+2)
			{

				hitPosition temp = { units,check };
				possibleHits.push_back(temp);

			}
			if (*units > 2)
			{
				side = 0;
				int* enemy = board.DiagonalMovement(units, side, !identityCheck);
				int* check = board.DiagonalMovement(enemy, side, !identityCheck);
				if (check != nullptr && enemy!= nullptr &&*check == 0 && *enemy != identity && *enemy != 0 && *enemy != identity + 2)
				{
					hitPosition temp = { units,check };
					possibleHits.push_back(temp);



				}
				side = 1;
				enemy = board.DiagonalMovement(units, side, !identityCheck);
				check = board.DiagonalMovement(board.DiagonalMovement(units, side, !identityCheck), side, !identityCheck);
				if (check != nullptr && enemy!= nullptr &&*check == 0 && *enemy != identity && *enemy != 0 && *enemy != identity + 2)
				{

					hitPosition temp = { units,check };
					possibleHits.push_back(temp);

				}
			}

			
			
			
			
			
		}
		possibleHits.shrink_to_fit();
		return possibleHits.size();
	}
	bool MovementCheck(Board& board, int* units) 
	{
		bool side = 0;

		int* LOCALcheck = board.DiagonalMovement(units, side, identityCheck);

		if (LOCALcheck != nullptr && *LOCALcheck == 0 )
		{
			return 1;



		}
		side = 1;

		LOCALcheck = board.DiagonalMovement(units, side, identityCheck);

		if (LOCALcheck != nullptr && *LOCALcheck == 0)
		{

			return 1;
		}
		if (*units > 2)
		{


			LOCALcheck = board.DiagonalMovement(units, side, !identityCheck);

			if (LOCALcheck != nullptr && *LOCALcheck == 0)
			{
				return 1;


			}
			side = 0;
			LOCALcheck = board.DiagonalMovement(units, side, !identityCheck);
			if (LOCALcheck != nullptr && *LOCALcheck == 0)
			{
				return 1;
			}
		}
		return 0;
	}
	bool MovementCheckPub(Board& board, int* units, int* position)
	{
		bool side = 0;

		int* LOCALcheck = board.DiagonalMovement(position, side, identityCheck);
		if (units == LOCALcheck)
		{
			return 1;



		}
		side = 1;

		LOCALcheck = board.DiagonalMovement(position, side, identityCheck);

		if (units == LOCALcheck)
		{

			return 1;
		}
		if (*position > 2)
		{


			LOCALcheck = board.DiagonalMovement(position, side, !identityCheck);

			if (units == LOCALcheck)
			{
				return 1;


			}
			side = 0;
			LOCALcheck = board.DiagonalMovement(position, side, !identityCheck);
			if (units == LOCALcheck)
			{
				return 1;
			}
		}
		return 0;
	}
	bool MovementPossibility(Board& board)
	{
		for (int* units : unitPositions)
		{
			MovementCheck(board,units );
		}
		return unitPositions.size();
	}



	
	
	void PrintHits()
	{
		for (hitPosition N: possibleHits)
		{
			*N.HitPosition =2;
			*N.PlayerPosition = 1;
		}
	}
};
