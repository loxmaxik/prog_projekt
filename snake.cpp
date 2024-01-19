#include "snake.h"

snakepart::snakepart(int col,int row) {
	x=col;
	y=row;
}

snakepart::snakepart() {
	x=0;
	y=0;
}
snakeclass::snakeclass() {
	initscr();
	cbreak();
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	nodelay(stdscr,true); //getch will not wait until the user presses a key
	keypad(stdscr,true); //allow keypad and f-keys
	noecho(); // won't show the characters typed
	curs_set(0); //cursor invisible
	getmaxyx(stdscr,maxheight,maxwidth);
	partchar='x'; //snake char
	oldalchar=(char)218; //border
	foo='*'; //food char
	food.x=0; // food position
	food.y=0; //food position
	again=0;
#ifdef _WIN32
	{
		HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
		DWORD mode;
		GetConsoleMode(hStdin, &mode);
		SetConsoleMode(hStdin, mode & ~(ENABLE_QUICK_EDIT_MODE | ENABLE_INSERT_MODE));
	}
#endif







}

snakeclass::~snakeclass() {
	nodelay(stdscr,false);
	getch();
	endwin();




}

void snakeclass::putfood() {
	while(1) {

		int tmpx=rand()%maxwidth+1;
		int tmpy=rand()%maxheight+1;
		for(int i=0; i<snake.size(); i++)
			if(snake[i].x==tmpx && snake[i].y==tmpy)
				continue;
		if(tmpx>=maxwidth-2 || tmpy>=maxheight-3)
			continue;
		food.x=tmpx;
		food.y=tmpy;
		break;

	}
	mvaddch(food.y,food.x,foo);

	refresh();
}

bool snakeclass::collision() {
	if(snake[0].x==1 || snake[0].x==maxwidth-1 || snake[0].y==1 || snake[0].y==maxheight-2)
		return true;

	for (int i=2; i<snake.size(); i++)
		if(snake[0].x==snake[i].x && snake[0].y==snake[i].y)
			return true;

	if(snake[0].x==food.x && snake[0].y==food.y) {
		get=true;
		putfood();
		points+=10;
		mvprintw(maxheight-1,0,"%d", points);
		if((points%100)==0)
			del-=10000;


	} else
		get=false;
	return false;


}

void snakeclass::movesnake() {
	int tmp=getch();
	switch(tmp) {
		case 'a' :
		case 'A':
		case KEY_LEFT  :
			if(direction!='r')
				direction='l';
			break;
		case 'd':
		case 'D':
		case KEY_RIGHT:
			if(direction!='l')
				direction='r';
			break;
		case 'w':
		case 'W':
		case KEY_UP:
			if(direction!='d')
				direction='u';
			break;
		case 's':
		case 'S':
		case KEY_DOWN:
			if(direction!='u')
				direction='d';
			break;
		case KEY_BACKSPACE:
			direction='q';
			break;
	}
	if(!get) {
		move(snake[snake.size()-1].y,snake[snake.size()-1].x);
		addch(' ');

		snake.pop_back();
	}
	if(direction=='l')
		snake.insert(snake.begin(),snakepart(snake[0].x-1,snake[0].y));
	else if(direction=='r')
		snake.insert(snake.begin(),snakepart(snake[0].x+1,snake[0].y));
	else if(direction=='u')
		snake.insert(snake.begin(),snakepart(snake[0].x,snake[0].y-1));
	else if(direction=='d')
		snake.insert(snake.begin(),snakepart(snake[0].x,snake[0].y+1));

	mvaddch(snake[0].y,snake[0].x,partchar);
	refresh();

}
void savescore() {









}
void drawbox(int x1, int y1, int x2, int y2, char oldalchar) {

	for(int i=x1; i<=x2; i++) {
		mvaddch(y1,i,oldalchar);

	}
	for(int i=x1; i<=x2; i++) {
		mvaddch(y2,i,oldalchar);

	}
	for(int i=y1; i<=y2; i++) {
		mvaddch(i,x1,oldalchar);
		mvaddch(i,x2,oldalchar);
	}


}
void scores()		{
	std::string line;
	std::ifstream file("scores.txt");
	int i=1;
	if (file.is_open()) {
		while (getline(file, line)) {
			std::cout << i <<". "<<line << "\n";
			i++;
		}
		file.close();
	}


}

void snakeclass::start() {


	for(int i=0; i<5; i++)
		snake.push_back(snakepart(40+i, 10));
	points=0; //score
	del=110000; // delay time
	get=false;
	direction='l';

	drawbox(0,0, maxwidth-1, maxheight-2, oldalchar);
	for(int i=0; i<snake.size(); i++) {
		mvaddch(snake[i].y, snake[i].x, partchar);




	}
	putfood();

	mvprintw(maxheight-1,0,"%d",points);

	mvaddch(food.y,food.x,foo);
	refresh();
	while(1) {
		if(collision()) {
			nodelay(stdscr,false);

			snake.clear();
			clear();



			mvprintw(10,(maxwidth-12)/2, "game over");
			mvprintw(12,(maxwidth-12)/2, "score: %d", points);
			mvaddstr(16,(maxwidth-11)/2, "play again?");
			attron(COLOR_PAIR(1));
			mvprintw(19,(maxwidth-11)/2, " yes ");
			mvprintw(19,(maxwidth-11)/2+7, " no ");
			attroff(COLOR_PAIR(1));
			refresh();
			nodelay(stdscr,false);
			while(1) {
				int hcar = getch();
				if(hcar == KEY_MOUSE) {
					MEVENT event1;
					if (getmouse(&event1) == OK)
						if (event1.bstate & BUTTON1_CLICKED) {
							if(event1.x >= (maxwidth-11)/2 && event1.x<=(maxwidth-11)/2+4 && event1.y == 19 ) {
								again=1;
								break;
							}
							if(event1.x >= (maxwidth-11)/2+7 && event1.x<=(maxwidth-11)/2+10 && event1.y == 19 ) {
								again=0;
								break;
							}
						}
				}
			}
			clear();
			break;

		}

		movesnake();
		if(direction=='q')
			break;
		if(direction=='u' || direction=='d')
			usleep(del*2);
		else
			usleep(del);


	}



}

void snakeclass::printmenu() {
	while(1==1) {



		if(again) {
			again=0;
			clear();
			nodelay(stdscr,true);
			start();
		} else



			mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
		attron(COLOR_PAIR(1));
		mvprintw(0,maxwidth-3, "x");
		mvprintw( 5,15," Start game ");
		attroff(COLOR_PAIR(1));

		mvprintw(maxheight-22,maxwidth-60,"          /^\\/^\\");
		mvprintw(maxheight-21,maxwidth-60,"        _|__|  O|  ");
		mvprintw(maxheight-20,maxwidth-60,"\\/     /~     \\_/ \\");
		mvprintw(maxheight-19,maxwidth-60," \\____|__________/  \\");
		mvprintw(maxheight-18,maxwidth-60,"        \\_______     \\");
		mvprintw(maxheight-17,maxwidth-60,"                `\\     \\                 \\");
		mvprintw(maxheight-16,maxwidth-60,"                  |     |                  \\");
		mvprintw(maxheight-15,maxwidth-60,"                 /      /                    \\");
		mvprintw(maxheight-14,maxwidth-60,"                /     /                       \\\\");
		mvprintw(maxheight-13,maxwidth-60,"              /      /                         \\ \\");
		mvprintw(maxheight-12,maxwidth-60,"             /     /                            \\  \\");
		mvprintw(maxheight-11,maxwidth-60,"           /     /             _----_            \\   \\");
		mvprintw(maxheight-10,maxwidth-60,"          /     /           _-~      ~-_         |   |");
		mvprintw(maxheight-9,maxwidth-60,"         (      (        _-~    _--_    ~-_     _/   |");
		mvprintw(maxheight-8,maxwidth-60,"          \\      ~-____-~    _-~    ~-_    ~-_-~    /");
		mvprintw(maxheight-7,maxwidth-60,"            ~-_           _-~          ~-_       _-~");
		mvprintw(maxheight-6,maxwidth-60,"               ~--______-~                ~-___-~ ");


		int tmpch = getch();


		if(tmpch == KEY_MOUSE) {
			MEVENT event;
			if (getmouse(&event) == OK)
				if (event.bstate & BUTTON1_CLICKED) {
					if(event.x == maxwidth-3 && event.y == 0 )
						break;
					else if(event.x >= 15&& event.y== 5 && event.x<=26) {
						clear();
						nodelay(stdscr,true);
						start();
					}
				}
		}
	}
}