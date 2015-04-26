#include "KeyListener.h"

KeyBoardGUI::KeyBoardGUI() : ROWS(3), KEYS_PER_ROW(10)
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
void KeyBoardGUI::renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
	SDL_Rect *clip)
{
	SDL_RenderCopy(ren, tex, clip, &dst);
}

void KeyBoardGUI::changeColours(char letter, SDL_Color color)
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

void KeyBoardGUI::renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
	SDL_Rect *clip)
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

SDL_Texture* KeyBoardGUI::renderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer)
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
void KeyBoardGUI::CreateWindow()
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

void KeyBoardGUI::DrawKey(Key& key)
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

void KeyBoardGUI::DrawHandPosition(int handIndex, int row)
{
	int i = 0;
	int limit = 0;
	if (handIndex)
	{
		i = KEYS_PER_ROW / 2;
		limit = KEYS_PER_ROW;
	}
	else
	{
		i = 0;
		limit = KEYS_PER_ROW / 2;
	}
	for (int j = ROWS - 1; j >= 0; j--)
	{
		for(int k = i; k < limit; k++)
		{
			if (j == ROWS - row - 1)
			{
				if (!(keys[j][k].color.r == 255 && keys[j][k].color.g == 0 && keys[j][k].color.b == 0 && keys[j][k].color.a == 255))
				{
					changeColours(keys[j][k].symbol[0], KeyListener::GREEN);
				}
					
			}
			else
				changeColours(keys[j][k].symbol[0], KeyListener::white);
		}
			
	}
		
	SDL_RenderPresent(ren);
}

void KeyBoardGUI::DrawKeyBoard()
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


bool KeyBoardGUI::AreColorsEqual(SDL_Color one, SDL_Color other) const
{
	return (one.a == other.a &&
			one.b == other.b &&
			one.r == other.r &&
			one.g == other.g);
}

void KeyBoardGUI::RedrawEmptyBoard()
{
	SDL_RenderClear(ren);
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < KEYS_PER_ROW; j++)
		{
			// delete
			if (!AreColorsEqual(keys[i][j].color, KeyListener::GREEN))
			{
				if (AreColorsEqual(keys[i][j].color, KeyListener::red))
				{
					keys[i][j].color.g = keys[i][j].color.a = 255;
					keys[i][j].color.r = keys[i][j].color.b = 0;
				}
				else
					keys[i][j].color.r = keys[i][j].color.g = keys[i][j].color.b = keys[i][j].color.a = 255;				
			}
			DrawKey(keys[i][j]); // This function sets the pointer of SDL_Texture too.... (bad).
		}
	}
}
