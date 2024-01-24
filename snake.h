#include <cstdlib>
#include <iostream>
#include <vector>
#include <ncursesw/curses.h>
#include <ctime>
#include <fstream>
#ifdef _WIN32 // Sprawdzenie, czy kod jest kompilowany na Windows
#include <windows.h>
#endif

#ifndef SNAKE_H
#define SNAKE_H

struct objectpart {
	int x,y, type;
	objectpart(int col,int row, int type);
	objectpart();
};

class snakeclass {
	WINDOW *settingsWin;
	int score, delay, maxwidth, maxheight, settingswidth=60, settingsheight=20, color, color2, fruit ;
	objectpart food [3];
	char partchar, borderchar, foodchar;
	bool get, again, quit;
	std::vector<objectpart> snake, snake2, mine;
	void putmine();
	void putfood(int x1,int y1 ,int x2,int y2, int num, int type);
	bool collision(int x2,int y2);
	void movesnake();
 
    enum way{
    	up,		down,	left,	right
	};
	way direction;
	
	enum speed{
		slow,	average,	fast				
	};
	speed snakespeed;
	
	enum gamemode{
		normal,		walls,		mines,		ying,		infinite
	};
    gamemode mode;
	
public:	

	WINDOW *create_newwin(int, int, int, int);
	void destroy_win(WINDOW *local_win);
	snakeclass();
	~snakeclass();
	void initialize();
	
	void snakeimg(int x , int y );
	void drawbox(int x1, int y1, int x2, int y2, char borderchar);
	
	void printmenu();
	void settings();
	
	void start();

	void startinf();
	void movefield();

};
#endif