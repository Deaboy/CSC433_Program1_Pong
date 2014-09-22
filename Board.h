#ifndef _BOARD_H_
#define _BOARD_H_

#include <string>
#include "Drawable.h"
using namespace std;

class Board : public Drawable
{
	private:
		int x;
		int y;
		int width;
		int height;
		int border;
		unsigned char* left_text;
		unsigned char* right_text;

	public:
		Board(int x, int y, int width, int height, int border);
		~Board();
		void setLeftText(string text);
		void setRightText(string text);

		int getX();
		int getY();
		int getWidth();
		int getHeight();

		void draw();
};

#endif