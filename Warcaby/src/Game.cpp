#include "Log.h"
#include <array>
#include "Board.h"
#include "Player.h"
#include <windows.h>
#include "GUI.h"
#include"Console.h"

	enum
	{
		left, right
	};
	enum
	{
		up, down
	};
int* Select(Board& board, Player& player,bool movetype);

//bool MainMovement(Player& player,Board& board, int* position)
//{
//
//	system("cls");
//	board.Display();
//	player.CountUnits(board);
//	player.HitPossibility(board);
//	if (player.possibleHits.size())
//	{
//		bool control = 0;
//		do
//		{
//				//int* position = Select(board, player, 0);
//			for (auto& check : player.possibleHits)
//			{
//				//if (check.PlayerPosition == position)
//				//{
//				//	LOG("Correct Unit Selection");
//				//	int* target = nullptr;
//				//	do
//				//	{	
//				//		//target = Select(board, player, 1);
//				//	} while (check.HitPosition != target);
//				//	LOG("Correct Target Selection");
//				//	player.Move(position, target);
//				//	*(int*)(((int)position + (int)target) / 2) = 0; //average value of positions;
//				//	control = 1;
//				//	break;
//				//}
//
//
//			}
//			if (control)
//			{
//				player.CountUnits(board);
//				
//				LOG("Hit Made");
//				if (player.HitPossibility(board))
//				{
//					system("cls");
//					board.Display();
//					control = 0;
//				}
//				
//			}
//			else
//				LOG("Selection Incorrect, you have to make a hit!");
//		} while (!control);
//		return 1;
//	}
//	else if (player.unitPositions.size())
//	{
//		bool control = 0;
//		do 
//		{
//			int* position = Select(board, player, 0);
//			if (player.MovementCheck(board, position))
//			{	
//				while (true)
//				{
//				int* target = Select(board, player, 1);
//				if (player.MovementCheckPub(board, target, position))
//				{
//					player.Move(position, target);
//					LOG("Move Succesfull!");
//					return 1;
//				}
//				else
//					LOG("You can't move there!");
//				}
//			}				
//			else
//				std::cout << "This unit can't move!" << std::endl;
//
//			
//			
//
//		} while (!control);
//	}
//	else
//		return 0;
//
//}
void PlayersTurn(Player& player, Board& board)
{
	player.CountUnits(board);


	if (player.MovementPossibility(board) || player.HitPossibility(board))
	{	//fix
		//MainMovement(player, board);
		board.Display();
	}
	else
	{
		std::cout << "Player " << player.identity << " can't move!"<<std::endl;
	}
}

bool LoseCondition(Player& player,Board& board)
{	
	
	if (player.MovementPossibility(board))
	{

		return 0;
	}
	else
		return 1;

	
}
Field* PressedField(SDL_Event &e)
{
	int x = 0;
	int y = 0;

	 	 Field* currentfield  = MouseToField(x, y, side);
	
			 //SDL_Rect SizeOfField = { currentfield->posx ,currentfield->posy ,side / 8,side / 8 };
			// SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
			// SDL_RenderFillRect(gRenderer, &SizeOfField);
			 return currentfield;

		 

}
enum
{
	SELECT = 0, MOVE = 1, NEUTRAL = 0
};
bool MainMovement(Board& board, Player& player, Field* &currentfield, Field* &previousfield, bool selectiontype,SDL_Event &e,int &turnCounter, std::vector<Field*> &Hits)
{	
	player.CountUnits(board);
	player.HitPossibility(board);
	
	currentfield = PressedField(e);
	if (player.possibleHits.size() && selectiontype == SELECT)
	{
		
		for (auto& units : player.possibleHits)
		{
				
			if (currentfield->relatedPosition == units.PlayerPosition)
			{
				
				
				if (board.DiagonalMovement(board.DiagonalMovement(currentfield->relatedPosition, left, up), left, up) == units.HitPosition)
					Hits.push_back(currentfield - 14);
				if (board.DiagonalMovement(board.DiagonalMovement(currentfield->relatedPosition, right, up), right, up) == units.HitPosition)
					Hits.push_back(currentfield - 18);
				if (board.DiagonalMovement(board.DiagonalMovement(currentfield->relatedPosition, left, down), left, down) == units.HitPosition)
					Hits.push_back(currentfield + 18);
				if (board.DiagonalMovement(board.DiagonalMovement(currentfield->relatedPosition, right, down), right, down) == units.HitPosition)
					Hits.push_back(currentfield + 14);
			
				
				return 1;
			}
			
		}
				return 0;
	}
	else if (player.possibleHits.size() && selectiontype == MOVE)
	{
		
		for (auto& units : player.possibleHits)
		{
			if (previousfield->relatedPosition== units.PlayerPosition && currentfield->relatedPosition == units.HitPosition )
			{
				*(currentfield->relatedPosition) = *(previousfield->relatedPosition);
				*(previousfield->relatedPosition) = 0;
				board.zeroMiddle(previousfield->relatedPosition, currentfield->relatedPosition);
				Hits.clear();

				player.CountUnits(board);
				player.upgrade(board.A1);
				if (player.HitPossibility(board))
				turnCounter -= 2;

				return 1;
			}
			
		}
				return 0;
	}
	else if (selectiontype == SELECT)
	{
		
		if (*(currentfield->relatedPosition)%2 == player.identity%2 && *(currentfield->relatedPosition)  != 0 && player.MovementCheck(board,currentfield->relatedPosition))
			return 1;
		else
			return 0;
	}
	else
	{
		
		if (*(currentfield->relatedPosition) == 0 && player.MovementCheckPub(board, currentfield->relatedPosition, previousfield->relatedPosition))
		{
			*(currentfield->relatedPosition) = *(previousfield->relatedPosition);
			*(previousfield->relatedPosition) = 0;
			player.CountUnits(board);
			player.upgrade(board.A1);
			return 1;
		}
		else
			return 0;
	}

}

int main(int argc, char* argv[])
{
	HideConsole();
	Board board;
	board.Start();
	//board.Display();
	Player playerOne(1);
	Player playerTwo(2);
	
	{


		if (!Init())
		{
			printf("Failed to initialize");

		}
		else
		{
			if (!loadMedia())
			{
				printf("Media couldnt be loaded");
			}
			else
			{
				std::vector<Field*> Hits;
				SDL_Rect ScreenFormat = { 0,0,side,side };
				SetField(board);
				bool quit = false;
				SDL_Event e;
				SDL_Rect SizeOfField;
				Field* currentfield=NULL;
				Field* previousfield = NULL;
				int* positionSelected = NULL;
				int* positionTargeted = NULL;
				int turnCount = 0;
				enum
				{
					PLAYERONE_SELECT,PLAYERONE_MOVE,PLAYERTWO_SELECT,PLAYERTWO_MOVE
				};
				playerOne.CountUnits(board);
				playerTwo.CountUnits(board);
				while (!quit)
				{
					
					gBoard.render(NULL, &ScreenFormat);
					
					RenderUnits();
					while (SDL_PollEvent(&e) != 0)
					{
						
						//User requests quit
						switch (e.type)
						{
						case SDL_QUIT:
						{
							quit = true;
						}
						break;
						case SDL_MOUSEBUTTONDOWN:
							if (e.type == SDL_MOUSEBUTTONDOWN)
							{
								switch(e.button.button)
								{
								case SDL_BUTTON_LEFT:
								{
									bool selectionFlag = false;
									switch (turnCount % 4)
									{
									case PLAYERONE_SELECT:
										selectionFlag = MainMovement(board, playerOne, currentfield, previousfield, SELECT, e, turnCount,Hits);
										Hits.shrink_to_fit();
										break;
									case PLAYERONE_MOVE:
										selectionFlag = MainMovement(board, playerOne, currentfield, previousfield, MOVE, e, turnCount,Hits);

										currentfield = NULL;
										break;
									case PLAYERTWO_SELECT:
										selectionFlag = MainMovement(board, playerTwo, currentfield, previousfield, SELECT, e, turnCount,Hits);
										break;
									case PLAYERTWO_MOVE:
										selectionFlag = MainMovement(board, playerTwo, currentfield, previousfield, MOVE, e, turnCount,Hits);
										currentfield = NULL;
										break;
									}
									//board.Display();


									if (selectionFlag != 0)
									{
										turnCount++;
										if (currentfield != NULL)
										{
											SizeOfField = { currentfield->posx ,currentfield->posy ,side / 8,side / 8 };
										}
										else
										{
											SizeOfField = { 0,0,0,0 };
										}
										previousfield = currentfield;

									}

								}
								break;
								case SDL_BUTTON_RIGHT:
								{
									turnCount -= turnCount % 2;
									SizeOfField = { 0,0,0,0 };
								}
								break;
								}
								

							}
							break;
						}
							
					}
					for (auto redfields : Hits)
					{
						redfields->renderHits();
					}
					
					if (turnCount % 4 == PLAYERONE_SELECT&& LoseCondition(playerOne, board))
					{
						
						BlackWins.renderFscreen();
					}
					if (turnCount % 4 == PLAYERTWO_SELECT&& LoseCondition(playerTwo, board))
					{
						
						WhiteWins.renderFscreen();
					}
						
					BoardFields[8].renderHits();
					SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
						SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0x45);
						SDL_RenderFillRect(gRenderer, &SizeOfField);
					//SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					//Field* currentfield = MouseToField(x, y, side);//->UnitVisual->render(x,y);
					
					//SDL_Delay(200);
					SDL_RenderPresent(gRenderer);
					

				}

			}
		}
		close();
		return 0;
	}
	//int X = 1;
	//char Y = 'H';
	//int X2 = 6;
	//char Y2 = 'F';
	//LOG(board.PositionConv(X, Y));
	//LOG(board.A1);
	//board.ChangeSymbol(board.PositionConv(X,Y), 3);
	
	/*playerOne.CountUnits(board);
	playerOne.HitPossibility(board);
	playerOne.PrintHits();*/
	
	
	//playerTwo.HitPossibility(board);
	//playerTwo.PrintHits();


	//while (true)
	//playerOne.CheckMovement(Select(board, playerOne),board.PositionConv(X,Y), board, true);
	//LOG(*Select(board, playerOne));
	//LOG(playerOne.unitCount);
	//playerOne.PrintHits();
	
	//WORKING CODE
	/*do
	{
		PlayersTurn(playerOne, board);
		PlayersTurn(playerTwo, board);

	} while ((playerOne.MovementPossibility(board) || playerOne.possibleHits.size() )&& (playerTwo.MovementPossibility(board) || playerTwo.possibleHits.size()));
	system("cls");
	EndMessage(playerOne, playerTwo, board);

	std::cin.get();*/
}

		
			
			