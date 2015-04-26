#pragma once

#include <iostream>
#include <SDL.h>
#include <string>
#include <vector>
#include "SDL_ttf.h"
#include "KeyListener.h"

class KeyListener;

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
	KeyBoardGUI();
	void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
		SDL_Rect *clip = nullptr);
	void changeColours(char letter, SDL_Color color);

	void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
		SDL_Rect *clip = nullptr);

	SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer);
	void CreateWindow();

	void DrawKey(Key& key);
	void DrawHandPosition(int handIndex, int row);
	void DrawKeyBoard();

	void RedrawEmptyBoard();
	bool AreColorsEqual(SDL_Color, SDL_Color) const;

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