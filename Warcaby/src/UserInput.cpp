#include "Log.h"
#include "Board.h"
#include "Player.h"
#include <limits>
void ClearStream()	
{	
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	LOG("Input Incorrect, Stream Cleared");
}


int* Select(Board& board,Player& player, bool movetype) //0 - select own, 1 - select target;
{	
	char Y= 'I'; //Row identifier
	int X = 9; //Column identifier
	std::cout << "Choose a field"<<std::endl;
	bool control = 0;
	int* selectedPosition=nullptr;
	while (!selectedPosition)
	{
		while (!control)
		{
			std::cout << "Choose a row (A - H)" << std::endl;
			std::cin >> Y;
			Y = Y - 32 * (Y > 96 && Y < 105);//conversion to uppercase letters
			if (std::cin.fail() || (Y < 65 || Y>72))

			{
				std::cout << "Row Incorrect - pick a letter from A to H"<<std::endl;
				ClearStream();
				continue;
			}
		
			else
			{					
				Y = Y - 32 * (Y > 96 && Y < 105);
				while (!control)
				{
					std::cout << "Choose a column (1 - 8)" << std::endl;
					std::cin >> X;
					if (std::cin.fail() || (X < 1 || X>8))
					{
						std::cout << "Column Incorrect - pick a number from 1 to 8" << std::endl;
						ClearStream();
						continue;
					}

					else
					{
						
						control = 1;
					}
				}

			}
			


		}
 		selectedPosition = board.PositionConv(X, Y);
		if ((*selectedPosition != player.identity && *selectedPosition != player.identity + 2&& !movetype) || (movetype && *selectedPosition != 0) )
		{
			std::cout << "You can't select this field"<<std::endl;
			selectedPosition = nullptr;
			control = 0;
		}
		else
		{
			if (!movetype)
			{
				std::cout << "This unit belongs to you" << std::endl;
			}
			else
			{
				std::cout << "Attempting movement" << std::endl;
			}
			return selectedPosition;
		}

	}

	return nullptr;
}