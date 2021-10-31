#pragma once
#include <SDL.h>
#include <string>
#include <iostream>
#include "Board.h"
static int SCREENHEIGHT = 400;
static int SCREENWIDTH = 480;
static int side;
static SDL_Window* gWindow = NULL;
static SDL_Renderer* gRenderer = NULL;
//The surface contained by the window
static SDL_Surface* gScreenSurface = NULL;

class LTexture



{	//actual texture
	int mWidth = 0;
	int mHeight = 0;
public:
	SDL_Texture* mTexture = NULL;
	void free();
	LTexture();
	~LTexture();

	//loads textures form a file
	bool loadTexture(std::string);
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void render(SDL_Rect* texturePart, SDL_Rect* render);
	void render(int x, int y);
	void renderFscreen();
	int getWidth();
	int getHeight();
	//void setBlendingMode(SDL_BlendMode blendtype);

};

static LTexture Unit;
static LTexture gBoard;
static LTexture UnitUpgrade;
static LTexture BlackWins;
static LTexture WhiteWins;

bool LTexture::loadTexture(std::string path)
{	//success flag
	free();
	 
	//load texture from a file
	 SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0x99, 0xd9, 0xea));
	if (loadedSurface == NULL)
	{
		printf("Failed to load file %s, SDL_error:%s", path.c_str(), SDL_GetError());

	}
	else
	{
		mTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (mTexture == NULL)
		{
			printf("Failed to create texture, SDL_error:%s", SDL_GetError());

		}
		else
		{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}
	return mTexture != NULL;
}
void LTexture::render(SDL_Rect* texturePart, SDL_Rect* render)
{
	//Set rendering space and render to screen

	SDL_RenderCopy(gRenderer, mTexture, texturePart, render);

}
void LTexture::render(int x, int y)
{
	SDL_Rect render = { x,y,getWidth(),getHeight() };

	SDL_RenderCopy(gRenderer, mTexture, NULL, &render);
	//printf("Rendering");
}
void LTexture::renderFscreen()
{
	SDL_Rect render = { 0,0,side,side };

	SDL_RenderCopy(gRenderer, mTexture, NULL, &render);
	//printf("Rendering");
}
void LTexture::free()
{
	SDL_DestroyTexture(mTexture);
	mTexture = NULL;
}
LTexture::~LTexture()
{	//Deallocate
	free();
}
LTexture::LTexture()
{
	mWidth = 0;
	mHeight = 0;
	mTexture = NULL;
}
void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}
int LTexture::getWidth()
{
	return mWidth;
}
int LTexture::getHeight()
{
	return mHeight;
}
//void LTexture::setBlendingMode(SDL_BlendMode blendtype)
//{
//	SDL_SetTextureBlendMode(mTexture, blendtype);
//}

class Field
{
public:
	
	
	int posx;
	int posy;
	int sideofsquare = side/8;
	SDL_Rect field = { 0,0,side/8,side/8 };
	LTexture* UnitVisual;
	int* relatedPosition;
	Field(int x, int y, int* pos, void* UnitVisual);
	Field();
	
	void renderHits();

	//converts mouse clicks to position

};
Field::Field(int x, int y, int* pos, void* UnitVisualout)
{
	posx = x;
	posy = y;
	relatedPosition = pos;
	UnitVisual = (LTexture*) UnitVisualout;
	field.x = x;
	field.y = y;
}
Field::Field()
{

}
Field BoardFields[64];
void Field::renderHits()
{
	SDL_Rect temp = field;
	
	
	
	SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 200);
	SDL_RenderFillRect(gRenderer,&temp);
	//SDL_RenderPresent(gRenderer);
	
}

bool Init()
{	//success flag
	bool success = true;
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL couldnt be initialized, SDL_error:%s", SDL_GetError());
		success = false;
	}
	else
	{
		SDL_DisplayMode dm;
		if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
			SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
			success = false;

		}
		else
		{
			SCREENHEIGHT = dm.h- 70;
			SCREENWIDTH = dm.w;
			side = (((SCREENHEIGHT <= SCREENWIDTH) * SCREENHEIGHT + (SCREENWIDTH <= SCREENHEIGHT) * SCREENWIDTH)/8)*8;
			gWindow = SDL_CreateWindow("Chekers", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, side, side, SDL_WINDOW_SHOWN);
			if (gWindow == NULL)
			{
				printf("Window couldnt be created, SDL_error:%s", SDL_GetError());
				success = false;

			}
			else
			{
				gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
				if (gRenderer == NULL)
				{
					printf("Rednerer couldnt be created, SDL_error:%s", SDL_GetError());
					success = false;
				}
				else
				{

					gScreenSurface = SDL_GetWindowSurface(gWindow);
				}


			}
		}

	}
	return success;
}

void SetField(Board &board)
{
	int x = 0;
	int y = 0;
	
	

	for (int i = 0; i < 64; i++)
	{
		
		x = i%8 * side / 8;
		if (i%8==0&&i!=0)
		{
			x = 0;
			y += (side / 8);
		}
		BoardFields[i] = { x,y,(int*)(board.A1 + 1 * i),(void*)&Unit };
		
		//std::cout << "y:" << y << "," << "x" << x<<std::endl;
		
	}
}

Field* MouseToField(int& x, int& y, int side)
{

	SDL_GetMouseState(&x, &y);

	//std::cout << (int)&BoardFields[x / (side / 8) + y / (side / 8) * 8] - (int)&BoardFields[0]<< " SPACE ";
	//std::cout << (int)BoardFields[x / (side / 8) + y / (side / 8) * 8].relatedPosition - (int)BoardFields[0].relatedPosition << std::endl;
	return &BoardFields[x / (side / 8) + (y / (side / 8)) * 8];
}
Field* PressedField(SDL_Event& e)
{
	int x = 0;
	int y = 0;

	Field* currentfield = MouseToField(x, y, side);

	//SDL_Rect SizeOfField = { currentfield->posx ,currentfield->posy ,side / 8,side / 8 };
   // SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
   // SDL_RenderFillRect(gRenderer, &SizeOfField);
	return currentfield;



}
#define WHITE 255,255,255
#define BLACK 30,30,30
void RenderUnits()
{
	
	LTexture* UnitUpptr = &UnitUpgrade;
	for (int i = 0; i < 64;i++)
	{
		int x=0, y=0;
	//	SDL_Rect Unitsize = { x,y,side / 8,side / 8 };
		//Unitsize.x = BoardFields[i].posx;

		//Unitsize.y = BoardFields[i].posy;
		switch (*(BoardFields[i].relatedPosition))
		{
		case 0:
			//Unitsize = { 0,0,0,0 };
			break;
		case 1:
			BoardFields[i].UnitVisual->setColor(WHITE);
			BoardFields[i].UnitVisual = &Unit; 
			BoardFields[i].UnitVisual->render(NULL, &BoardFields[i].field);
			break;
		case 2:
			BoardFields[i].UnitVisual->setColor(BLACK); 
			BoardFields[i].UnitVisual = &Unit;
			BoardFields[i].UnitVisual->render(NULL, &BoardFields[i].field);
			break;
		case 3:
		{BoardFields[i].UnitVisual = &UnitUpgrade;
		BoardFields[i].UnitVisual->setColor(WHITE);
		BoardFields[i].UnitVisual->render(NULL, &BoardFields[i].field);
		}	break;
		case 4:
		{	BoardFields[i].UnitVisual = &UnitUpgrade;
		BoardFields[i].UnitVisual->setColor(BLACK);
		BoardFields[i].UnitVisual->render(NULL, &BoardFields[i].field);
		}
			break;
		}
		
		
		/*	BoardFields[i].UnitVisual->setColor(255, 255, 255);
			if (*(BoardFields[i].relatedPosition) == 2)
				BoardFields[i].UnitVisual->setColor(0, 0, 0);*/

			//BoardFields[i].UnitVisual->render(NULL,&BoardFields[i].field);
		
	}
}

bool loadMedia()
{
	
	//success flag
	bool success = true;

	if (!gBoard.loadTexture("board.bmp"))
	{
		printf("Failed to load the board texture, SDL_error:%s", SDL_GetError());
		success = false;
	}
	if (!Unit.loadTexture("Unit.bmp"))
	{
		printf("Failed to load the Unit texture, SDL_error:%s", SDL_GetError());
		success = false;
	}
	if (!UnitUpgrade.loadTexture("UnitUpgrade.bmp"))
	{
		printf("Failed to load the UnitUpgrade texture, SDL_error:%s", SDL_GetError());
		success = false;
	}
	if (!BlackWins.loadTexture("BlackWin.bmp"))
	{
		printf("Failed to load the BlackWin texture, SDL_error:%s", SDL_GetError());
		success = false;
	}
	if (!WhiteWins.loadTexture("WhiteWin.bmp"))
	{
		printf("Failed to load the WhiteWin texture, SDL_error:%s", SDL_GetError());
		success = false;
	}
	
	return success;
}

void close()
{
	gBoard.free();

	gScreenSurface = NULL;
	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	gWindow = NULL;
	Unit.free();
	gBoard.free();
	UnitUpgrade.free();
	BlackWins.free();
	WhiteWins.free();
	SDL_Quit;


}

