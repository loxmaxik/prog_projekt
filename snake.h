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

struct snakepart {
	int x,y;
	snakepart(int col,int row);
	snakepart();
};

class snakeclass {
	int points, del, maxwidth, maxheight;
	char direction, partchar, oldalchar, foo;
	bool get, again;
	snakepart food;
	std::vector<snakepart> snake;
	void putfood();
	bool collision();
	void movesnake();
    
	
	
	
    //void printscore();
public:	

	snakeclass();
	~snakeclass();
	void start();
	void printmenu();
};
	
	
#endif
