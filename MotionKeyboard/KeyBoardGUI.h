#include <iostream>
#include <SDL.h>
#include <string>
#include <vector>
#include "SDL_ttf.h"


struct Key
{
	std::string symbol;
	int x;
	int y;
	SDL_Color color;

	SDL_Texture* texture;

	Key()
	{
		setDefaultValues();
	}

private:
	void setDefaultValues()
	{
		color.a = color.b = color.g = color.r = 255;
		x = y = 0;
		symbol = ' ';
		texture = NULL;
	}
};


class KeyBoardGUI
{
public:
	KeyBoardGUI() : ROWS(3), KEYS_PER_ROW(10)
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0){
			std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
			throw 1;
		}
		FONT = "arimo.ttf";
		letters = "qwertyuiopasdfghjkl;zxcvbnm,./";

		// TO DO

		FONT_SIZE = 32;

		screen_width = 1200;
		screen_height = 600;

		// Resize the table for the keyboard.
		keys.resize(ROWS);
		for (int i = 0; i < ROWS; ++i)
		{
			keys[i].resize(KEYS_PER_ROW);
		}
	}
	void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
		SDL_Rect *clip = nullptr)
	{
		SDL_RenderCopy(ren, tex, clip, &dst);
	}

	void changeColours(char letter, SDL_Color color)
	{
		for (int i = 0; i < ROWS; i++)
		{
			for (int j = 0; j < KEYS_PER_ROW; j++)
			{
				if (keys[i][j].symbol[0] == letter)
				{
					//delete keys[i][j].texture;
					keys[i][j].color = color;
					DrawKey(keys[i][j]);
				}
			}
		}
	}

	void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
		SDL_Rect *clip = nullptr)
	{
		SDL_Rect dst;
		dst.x = x;
		dst.y = y;
		if (clip != nullptr)
		{
			dst.w = clip->w;
			dst.h = clip->h;
		}
		else 
		{
			SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
		}
		renderTexture(tex, ren, dst, clip);
	}

	SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer)
	{
		//Open the font
		TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
		if (font == nullptr){

			return nullptr;
		}
		//We need to first render to a surface as that's what TTF_RenderText
		//returns, then load that surface into a texture
		SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
		if (surf == nullptr){
			TTF_CloseFont(font);
			return nullptr;
		}
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
		if (texture == nullptr){
			true;
		}
		//Clean up the surface and font
		SDL_FreeSurface(surf);
		TTF_CloseFont(font);
		return texture;
	}
	void CreateWindow()
	{
		win = SDL_CreateWindow("Hello world", 0, 0, screen_width, screen_height, SDL_WINDOW_SHOWN);
		if (win == nullptr)
		{
			std::cerr << SDL_GetError() << std::endl;
			SDL_Quit();
			return;
		}

		ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		if (ren == nullptr)
		{
			SDL_DestroyWindow(win);
			std::cerr << SDL_GetError() << std::endl;
			SDL_Quit();
			return;
		}

	}

	void DrawKey(Key& key)
	{
		key.texture = renderText(key.symbol, FONT,
			key.color, FONT_SIZE, ren);
		// TO DO
		if (key.texture == nullptr){
			TTF_Quit();
			SDL_Quit();
			return;
		}

		renderTexture(key.texture, ren, key.x, key.y);
	}

	void DrawHandPosition(int handIndex, int row)
	{

	}

	void DrawKeyBoard()
	{
		int cellHeight = screen_height / (ROWS + 3);		// First row empty , second row text, last row are empty.
		int cellWidth = screen_width / (KEYS_PER_ROW + 2);  // First and last column is empty.

		if (TTF_Init() != 0)
		{
			SDL_Quit();
			return;
		}
		SDL_RenderClear(ren);
		for (int i = 0; i < ROWS; i++)
		{
			for (int j = 0; j < KEYS_PER_ROW; j++)
			{
				keys[i][j].symbol = letters[i * KEYS_PER_ROW + j];
				keys[i][j].x = cellWidth * (1 + j + 0.5); // Skip the first column and centers the letters.
				keys[i][j].y = cellHeight * (2 + i + 0.5); // Skip the first two rows. ^
				DrawKey(keys[i][j]); // This function sets the pointer of SDL_Texture too.... (bad).
			}
		}

		// TO DO Delete one below.

		SDL_RenderPresent(ren);
	}

	void RedrawEmptyBoard()
	{
		SDL_RenderClear(ren);
		for (int i = 0; i < ROWS; i++)
		{
			for (int j = 0; j < KEYS_PER_ROW; j++)
			{
				keys[i][j].color.r = keys[i][j].color.g = keys[i][j].color.b = keys[i][j].color.a = 255;
				DrawKey(keys[i][j]); // This function sets the pointer of SDL_Texture too.... (bad).
			}
		}
	}

	SDL_Renderer* ren;
	SDL_Window* win;
	int screen_width;
	int screen_height;
	std::string FONT;
	size_t FONT_SIZE;
	std::string letters;
	std::vector<std::vector<Key>> keys;
	const int KEYS_PER_ROW;
	const int ROWS ;

};