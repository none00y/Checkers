#include <SDL.h>
#include <string>
#include <iostream>
static int SCREENHEIGHT= 400;
static int SCREENWIDTH= 480;
static int side;
static SDL_Window* gWindow = NULL;
static SDL_Renderer* gRenderer = NULL;
//The surface contained by the window
static SDL_Surface* gScreenSurface = NULL;

class LTexture
	
	
	
{	//actual texture
	SDL_Texture* mTexture =NULL;
	int mWidth = 0;
	int mHeight = 0;
public:
	void free();
	LTexture();
	~LTexture();
	
	//loads textures form a file
	bool loadTexture(std::string);
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void render(SDL_Rect* texturePart, SDL_Rect* render);
	void render(int x, int y);
	int getWidth();
	int getHeight();
	
};

static LTexture gBoard;
static LTexture* gBoardptr = &gBoard;

bool LTexture::loadTexture(std::string path)
	{	//success flag
		free();
		
		//load texture from a file
		SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
		if (loadedSurface == NULL)
		{
			printf("Failed to load file %s, SDL_error:%s", path.c_str(), SDL_GetError());
			
		}
		else
		{
			mTexture = SDL_CreateTextureFromSurface(gRenderer,loadedSurface);
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
		return mTexture!=NULL;
	}
void LTexture::render(SDL_Rect* texturePart ,SDL_Rect* render)
{
	//Set rendering space and render to screen

		SDL_RenderCopy(gRenderer, mTexture, texturePart, render);

}
void LTexture::render(int x,int y)
{
	SDL_Rect render = { x,y,getWidth(),getHeight() };

	SDL_RenderCopy(gRenderer, mTexture, NULL, &render);

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


class Field
{
public:
	
	int posx;
	int posy;
	LTexture* UnitVisual;
	int* relatedPosition;
	Field(int x, int y, int* pos,LTexture* UnitVisual);
	Field();


	//converts mouse clicks to position
	
};
Field::Field(int x, int y, int* pos, LTexture* UnitVisual)
{
	posx = x;
	posy = y;
	relatedPosition = pos;
	UnitVisual = UnitVisual;
}
Field::Field()
{

}
Field BoardFields[64];

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
			SCREENHEIGHT = dm.h;
			SCREENWIDTH = dm.w;
			side = (SCREENHEIGHT <= SCREENWIDTH) * SCREENHEIGHT + (SCREENWIDTH <= SCREENHEIGHT) * SCREENWIDTH;
			gWindow = SDL_CreateWindow("Chekers", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, side, side, SDL_WINDOW_SHOWN);
			if (gWindow == NULL)
			{
				printf("Window couldnt be created, SDL_error:%s", SDL_GetError());
				success = false;

			}
			else
			{
				gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
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


bool loadMedia()
{	
	//success flag
	bool success = true;
	
	if (!gBoard.loadTexture("board.bmp"))
	{
		printf("Failed to load the board texture, SDL_error:%s", SDL_GetError());
		success = false;
	}
	return success;
}

void close()
{
	gBoard.free();
	
	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	gWindow = NULL;

	SDL_Quit;


}

